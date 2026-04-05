// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/StorageSubsystem.h"

// Engine Headers
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/IStorageProvider.h"
#include "Interface/IStorageSettingsProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "SaveGame/Storage.h"



UStorage* UStorageSubsystem::GetStorage(const FName& StorageId)
{
	TObjectPtr<UStorage>* FoundStorage = StorageCollection.Find(StorageId);
	if (!FoundStorage)
	{
		return nullptr;
	}
	return FoundStorage->Get();
}

void UStorageSubsystem::LoadStorage(FStorageHandle&& Handle)
{
	if (!Handle.IsValid())
	{
		LOG_ERROR(LogStorage, TEXT("Invalid storage handle or storage is already loading"));
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	if (StorageCollection.Contains(Handle.StorageId))
	{
		LOG_WARNING(LogStorage, TEXT("Storage is already loaded"));
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Completed));
		return;
	}

	if (bNetLoad)
	{
		LoadStorage_Network(MoveTemp(Handle));
		return;
	}

	UStorage* Storage = LoadStorage_Internal(Handle.StorageId, Handle.StorageClass);
	ETaskState State = IsValid(Storage) ? ETaskState::Completed : ETaskState::Failed;

	Handle.Callback.ExecuteIfBound(FTaskResult(State));
}

void UStorageSubsystem::LoadStorageFromSettings(const UDeveloperSettings* Settings)
{
	const IStorageSettingsProvider* SettingsProvider = Cast<IStorageSettingsProvider>(Settings);
	if (!SettingsProvider)
	{
		return;
	}

	FStorageHandle Handle;
	Handle.StorageClass = SettingsProvider->GetStorageClass();
	Handle.StorageId = SettingsProvider->GetStorageId();
	//Handle.Url = SettingsProvider->GetStorageUrl();

	LoadStorage(MoveTemp(Handle));
}

void UStorageSubsystem::SaveStorage(const FName& StorageId)
{
	TObjectPtr<UStorage>* FoundStorage = StorageCollection.Find(StorageId);
	if (!FoundStorage)
	{
		return;
	}

	UStorage* Storage = FoundStorage->Get();
	if (!IsValid(Storage))
	{
		return;
	}

	if (bNetLoad && !Storage->GetForceSave())
	{
		return;
	}

	SaveStorage_Internal(Storage, StorageId);
}



bool UStorageSubsystem::MakeStorageId(TSubclassOf<UStorage> InStorageClass, const FString& InSlotName, int InUserIndex, FString& OutStorageId) const
{
	if (!IsValid(InStorageClass))
	{
		OutStorageId = TEXT_EMPTY;
		return false;
	}

	FString Combined = InStorageClass->GetPathName() + TEXT("::") + InSlotName + TEXT("::") + FString::FromInt(InUserIndex);
	uint32 Hash = GetTypeHash(Combined);

	OutStorageId = FString::Printf(TEXT("Storage%03d_%08X"), (InUserIndex + 1), Hash);

	return true;
}

void UStorageSubsystem::GetDefaultQuery(const FName& StorageId, TSharedPtr<FJsonObject>& QueryJson)
{
	QueryJson->SetStringField(TEXT("storageId"), StorageId.ToString());
}

void UStorageSubsystem::SerializeQuery(TSharedPtr<FJsonObject>& QueryJson, FString& OutString)
{
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutString);
	FJsonSerializer::Serialize(QueryJson.ToSharedRef(), Writer);
}



UStorage* UStorageSubsystem::CreateStorage_Internal(TSubclassOf<UStorage> StorageClass, const FName& StorageId)
{
	if (!IsValid(StorageClass))
	{
		LOG_ERROR(LogStorage, TEXT("Storage class is not valid"));
		return nullptr;
	}

	FString FileId;
	bool bSuccess = MakeStorageId(StorageClass, StorageId.ToString(), 0, FileId);
	if (!bSuccess)
	{
		LOG_ERROR(LogStorage, TEXT("Failed to make storage id"));
		return nullptr;
	}

	if (UGameplayStatics::DoesSaveGameExist(FileId, 0))
	{
		return Cast<UStorage>(UGameplayStatics::LoadGameFromSlot(FileId, 0));
	}

	UStorage* NewStorage = Cast<UStorage>(UGameplayStatics::CreateSaveGameObject(StorageClass));
	if (!IsValid(NewStorage))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage"));
		return nullptr;
	}

	NewStorage->InitializeDefaults();
	UGameplayStatics::SaveGameToSlot(NewStorage, FileId, 0);

	return NewStorage;
}

bool UStorageSubsystem::SaveStorage_Internal(UStorage* Storage, const FName& StorageId)
{
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogStorage, TEXT("Storage is not valid"));
		return false;
	}

	FString FileId;
	bool bSuccess = MakeStorageId(Storage->GetClass(), StorageId.ToString(), 0, FileId);
	if (!bSuccess)
	{
		LOG_ERROR(LogStorage, TEXT("Failed to make storage id"));
		return false;
	}

	Storage->DeinitializeStorage();

	return UGameplayStatics::SaveGameToSlot(Storage, FileId, 0);
}



void UStorageSubsystem::LoadStorage_Network(FStorageHandle Handle)
{
	if (!Handle.IsUrlValid())
	{
		LOG_ERROR(LogStorage, TEXT("Invalid url"));
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	TSharedPtr<FJsonObject> StorageQuery = MakeShared<FJsonObject>();
	GetDefaultQuery(Handle.StorageId, StorageQuery);

	FString RequestBody;
	SerializeQuery(StorageQuery, RequestBody);

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

	Request->SetURL(Handle.Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RequestBody);

	Request->OnProcessRequestComplete().BindUObject(this, &UStorageSubsystem::LoadStorage_NetworkResponse, MoveTemp(Handle));
	Request->ProcessRequest();
}

void UStorageSubsystem::LoadStorage_NetworkResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded, FStorageHandle Handle)
{
	if (!bSucceeded || !Response.IsValid())
	{
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	FString ResponseString = Response->GetContentAsString();

	TSharedPtr<FJsonObject> ResponseObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (!FJsonSerializer::Deserialize(Reader, ResponseObject))
	{
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	UStorage* Storage = LoadStorage_Internal(Handle.StorageId, Handle.StorageClass);
	if (!IsValid(Storage))
	{
		Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	if (ResponseObject.IsValid())
	{
		Storage->NetDeserialize(ResponseObject);
	}

	Handle.Callback.ExecuteIfBound(FTaskResult(ETaskState::Completed));
}

UStorage* UStorageSubsystem::LoadStorage_Internal(const FName& StorageId, TSubclassOf<UStorage> StorageClass)
{
	if (!StorageId.IsValid())
	{
		LOG_ERROR(LogInventory, TEXT("StorageId is not valid"));
		return nullptr;
	}

	IStorageProvider* StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogInventory, TEXT("Storage provider not found"));
		return nullptr;
	}

	UStorage* Storage = CreateStorage_Internal(StorageClass, StorageId);
	if (!IsValid(Storage))
	{
		LOG_ERROR(LogInventory, TEXT("Storage is not valid"));
		return nullptr;
	}

	StorageCollection.Add(StorageId, Storage);
	Storage->InitializeStorage();

	return Storage;
}



void UStorageSubsystem::OnPreGameInitialized()
{
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
}

void UStorageSubsystem::SaveAllStorages()
{
	for (const TPair<FName, UStorage*>& Pair : StorageCollection)
	{
		SaveStorage(Pair.Key);
	}
}

bool UStorageSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	return true;
}

void UStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UStorageSubsystem::OnPreGameInitialized);
}

void UStorageSubsystem::Deinitialize()
{
	SaveAllStorages();

	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
	Super::Deinitialize();
}



UStorageSubsystem* UStorageSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UStorageSubsystem* UStorageSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UStorageSubsystem>();
}


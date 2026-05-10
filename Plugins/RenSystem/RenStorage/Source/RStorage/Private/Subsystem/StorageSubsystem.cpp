// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/StorageSubsystem.h"

// Engine Headers
#include "GameFramework/SaveGame.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "Interface/StorageManager.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


int UStorageSubsystem::GetSlotIndex()
{
	return _SlotIndex;
}

void UStorageSubsystem::SetSlotIndex(int Index)
{
	_SlotIndex = FMath::Clamp(Index, 0, 50);
}


UObject* UStorageSubsystem::GetStorageManager(const FName& StorageId)
{
	TObjectPtr<UObject>* FoundManager = StorageCollection.Find(StorageId);
	if (!FoundManager)
	{
		return nullptr;
	}
	return FoundManager->Get();
}

void UStorageSubsystem::LoadStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback)
{
	if (!Definition.IsValid() || StorageCollection.Contains(Definition.StorageId))
	{
		LOG_ERROR(LogStorage, TEXT("Storage definition is invalid or already loaded"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	if (bRemoteLoad && bUseExperimental)
	{
		LoadRemoteStorage(Definition, MoveTemp(Callback));
	}
	else
	{
		LoadLocalStorage(Definition, MoveTemp(Callback));
	}
}

void UStorageSubsystem::SaveStorage(const FName& StorageId)
{
	UObject* FoundManager = GetStorageManager(StorageId);
	IStorageManager* Manager = Cast<IStorageManager>(FoundManager);
	if (!Manager)
	{
		LOG_ERROR(LogStorage, TEXT("Storage manager is invalid"));
		return;
	}

	USaveGame* SaveGame = Manager->GetStorage<USaveGame>();
	SaveLocalStorage(SaveGame, StorageId);
}


FString UStorageSubsystem::MakeUniqueStorageId(TSubclassOf<UObject> StorageClass, const FString& SlotName, int SlotIndex) const
{
	if (!IsValid(StorageClass) || SlotName.IsEmpty())
	{
		return TEXT_EMPTY;
	}

	FString Combined = StorageClass->GetPathName() + TEXT("::") + SlotName + TEXT("::") + FString::FromInt(SlotIndex);
	uint32 Hash = GetTypeHash(Combined);
	
	return FString::Printf(TEXT("Storage_Slot%03d_%08X"), (SlotIndex + 1), Hash);
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


TSubclassOf<USaveGame> UStorageSubsystem::GetStorageClass(const FStorageDefinition& Definition) const
{
	UClass* Class = Definition.StorageClass.Get();
	if (Class && Class->IsChildOf(USaveGame::StaticClass()))
	{
		return TSubclassOf<USaveGame>(Class);
	}
	return nullptr;
}

TSubclassOf<UObject> UStorageSubsystem::GetStorageManagerClass(const FStorageDefinition& Definition) const
{
	TSubclassOf<UObject> Class = Definition.ManagerClass.Get();
	if (Class && Class->ImplementsInterface(UStorageManager::StaticClass()))
	{
		return Class;
	}
	return nullptr;
}


UObject* UStorageSubsystem::CreateStorageManager(const FStorageDefinition& Definition)
{
	TSubclassOf<UObject> StorageManagerClass = GetStorageManagerClass(Definition);
	if (!IsValid(StorageManagerClass))
	{
		LOG_ERROR(LogStorage, TEXT("Storage manager class is invalid"));
		return nullptr;
	}

	UObject* StorageManager = NewObject<UObject>(this, StorageManagerClass);
	if (!IsValid(StorageManager))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage manager"));
		return nullptr;
	}

	return StorageManager;
}

USaveGame* UStorageSubsystem::CreateStorage(const FStorageDefinition& Definition, bool& bOutIsNew)
{
	FString StorageId = Definition.StorageId.ToString();
	TSubclassOf<USaveGame> StorageClass = GetStorageClass(Definition);

	FString UniqueId = MakeUniqueStorageId(StorageClass, StorageId, GetSlotIndex());
	if (UniqueId.IsEmpty())
	{
		LOG_ERROR(LogStorage, TEXT("Failed to make unique storage id"));
		return nullptr;
	}
	
	if (UGameplayStatics::DoesSaveGameExist(UniqueId, 0))
	{
		bOutIsNew = false;
		return UGameplayStatics::LoadGameFromSlot(UniqueId, 0);
	}

	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(StorageClass);
	if (!IsValid(SaveGame))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage"));
		return nullptr;
	}

	bOutIsNew = true;
	UGameplayStatics::SaveGameToSlot(SaveGame, UniqueId, 0);

	return SaveGame;
}

USaveGame* UStorageSubsystem::CreateStorage(const FStorageDefinition& Definition)
{
	bool bOutIsNew = false;
	return CreateStorage(Definition, bOutIsNew);
}


void UStorageSubsystem::LoadRemoteStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback)
{
	if (!Definition.IsValid() || !Definition.IsUrlValid())
	{
		LOG_ERROR(LogStorage, TEXT("Storage definition or url is not valid"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	TSharedPtr<FJsonObject> StorageQuery = MakeShared<FJsonObject>();
	GetDefaultQuery(Definition.StorageId, StorageQuery);

	FString RequestBody;
	SerializeQuery(StorageQuery, RequestBody);

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

	Request->SetURL(Definition.RemoteUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(RequestBody);

	Request->OnProcessRequestComplete().BindUObject(this, &UStorageSubsystem::HandleRemoteStorageResponse, Definition, Callback);
	Request->ProcessRequest();
}

void UStorageSubsystem::HandleRemoteStorageResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded, FStorageDefinition Definition, FTaskCallback Callback)
{
	FTaskResult Result(ETaskState::Failed);

	if (!bSucceeded || !Response.IsValid())
	{
		LOG_ERROR(LogStorage, TEXT("Failed to load storage"));
		Callback.ExecuteIfBound(Result);
		return;
	}

	FString ResponseString = Response->GetContentAsString();
	TSharedPtr<FJsonObject> ResponseObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
	if (!FJsonSerializer::Deserialize(Reader, ResponseObject))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to deserialize response"));
		Callback.ExecuteIfBound(Result);
		return;
	}

	if (!ResponseObject.IsValid())
	{
		LOG_ERROR(LogStorage, TEXT("Invalid response object"));
		Callback.ExecuteIfBound(Result);
		return;
	}

	USaveGame* SaveGame = CreateStorage(Definition);
	UObject* Manager = CreateStorageManager(Definition);
	IStorageManager* StorageManager = Cast<IStorageManager>(Manager);
	if (!IsValid(SaveGame) || !StorageManager)
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage or storage manager"));
		Callback.ExecuteIfBound(Result);
		return;
	}

	StorageCollection.Add(Definition.StorageId, Manager);

	StorageManager->SetStorage(SaveGame);
	StorageManager->LoadRemoteData(ResponseObject);
	StorageManager->OnStorageLoaded(false);

	Result.State = ETaskState::Completed;
	Callback.ExecuteIfBound(Result);
}


void UStorageSubsystem::LoadLocalStorage(const FStorageDefinition& Definition, FTaskCallback&& Callback)
{
	bool bIsNew = false;
	USaveGame* SaveGame = CreateStorage(Definition, bIsNew);
	UObject* Manager = CreateStorageManager(Definition);
	IStorageManager* StorageManager = Cast<IStorageManager>(Manager);
	if (!IsValid(SaveGame) || !StorageManager)
	{
		LOG_ERROR(LogStorage, TEXT("Storage or storage manager is invalid"));
		Callback.ExecuteIfBound(ETaskState(ETaskState::Failed));
		return;
	}

	StorageCollection.Add(Definition.StorageId, Manager);

	StorageManager->SetStorage(SaveGame);
	StorageManager->OnStorageLoaded(bIsNew);

	Callback.ExecuteIfBound(ETaskState(ETaskState::Completed));
}

bool UStorageSubsystem::SaveLocalStorage(USaveGame* SaveGame, const FName& StorageId)
{
	if (!IsValid(SaveGame))
	{
		LOG_ERROR(LogStorage, TEXT("SaveGame is not valid"));
		return false;
	}

	FString UniqueId = MakeUniqueStorageId(SaveGame->GetClass(), StorageId.ToString(), GetSlotIndex());
	if (UniqueId.IsEmpty())
	{
		LOG_ERROR(LogStorage, TEXT("Failed to make unique storage id"));
		return false;
	}

	return UGameplayStatics::SaveGameToSlot(SaveGame, UniqueId, 0);
}


void UStorageSubsystem::UnloadAllStorages()
{
	TArray<FName> StorageIds;
	StorageCollection.GetKeys(StorageIds);

	for (const FName& StorageId : StorageIds)
	{
		UnloadStorage(StorageId);
	}
}

void UStorageSubsystem::UnloadStorage(const FName& StorageId)
{
	IStorageManager* StorageManager = Cast<IStorageManager>(GetStorageManager(StorageId));
	if (!StorageManager)
	{
		LOG_ERROR(LogStorage, TEXT("Storage manager is invalid"));
		return;
	}

	USaveGame* SaveGame = StorageManager->GetStorage<USaveGame>();
	SaveLocalStorage(SaveGame, StorageId);

	StorageManager->OnBeginUnload();
	StorageManager->SetStorage(nullptr);
	StorageManager->OnAfterUnload();

	StorageCollection.Remove(StorageId);
}


bool UStorageSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	return true;
}

void UStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogStorage, TEXT("StorageSubsystem initialized"));
}

void UStorageSubsystem::Deinitialize()
{
	UnloadAllStorages();

	LOG_WARNING(LogStorage, TEXT("StorageSubsystem deinitialized"));
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


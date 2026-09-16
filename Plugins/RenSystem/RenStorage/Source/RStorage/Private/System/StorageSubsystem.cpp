// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/StorageSubsystem.h"

// Engine Headers
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "Core/Type/StorageArchive.h"
#include "Core/Type/StorageInstance.h"
#include "Data/StorageCache.h"
#include "Core/StorageManager.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


UObject* UStorageSubsystem::GetStorageManager(const FName& StorageId)
{
	TObjectPtr<UObject>* FoundManager = ManagerCollection.Find(StorageId);
	if (!FoundManager)
	{
		return nullptr;
	}
	return FoundManager->Get();
}

void UStorageSubsystem::LoadStorage(const FStorageDefinition& Definition, FOnStorageLoaded&& Callback)
{
	if (!StorageCache || !Definition.IsValid())
	{
		Callback.ExecuteIfBound(false);
		return;
	}

	UObject* ManagerObject = CreateStorageManager(Definition);
	IStorageManager* Manager = Cast<IStorageManager>(ManagerObject);
	UObject* Storage = NewObject<UObject>(GetGameInstance(), Definition.StorageClass, NAME_None, RF_Transient);
	if (!Manager || !IsValid(Storage))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage manager or storage object"));
		Callback.ExecuteIfBound(false);
		return;
	}

	bool bIsNewlyCreated = true;
	DeserializeStorage(Definition.StorageId, Storage, bIsNewlyCreated);

	Manager->SetStorage(Storage);
	Manager->OnStorageLoaded(bIsNewlyCreated);

	ManagerCollection.Add(Definition.StorageId, ManagerObject);

	Callback.ExecuteIfBound(true);
}

void UStorageSubsystem::SaveStorage(const FName& StorageId)
{
	IStorageManager* StorageManager = Cast<IStorageManager>(GetStorageManager(StorageId));
	if (!StorageManager)
	{
		LOG_ERROR(LogStorage, TEXT("Storage manager is invalid"));
		return;
	}

	SerializeStorage(StorageId, StorageManager->GetStorage());
}


void UStorageSubsystem::DeserializeStorage(const FName& StorageId, UObject* Storage, bool& bIsNewlyCreated)
{
	if (!IsValid(StorageCache) || !IsValid(Storage))
	{
		LOG_ERROR(LogStorage, TEXT("Failed to create storage manager or storage object"));
		return;
	}

	FString StorageClass = Storage->GetClass()->GetPathName();

	FStorageInstance* FoundInstance = StorageCache->Instances.FindByPredicate([StorageId, StorageClass](const FStorageInstance& Data) { return Data.StorageId == StorageId && Data.StorageClass == StorageClass; });

	if (FoundInstance && FoundInstance->Data.Num() != 0)
	{
		bIsNewlyCreated = false;
		FMemoryReader Reader(FoundInstance->Data, true);
		FStorageArchive Archive(Reader, true);
		Storage->Serialize(Archive);
	}
}

void UStorageSubsystem::SerializeStorage(const FName& StorageId, UObject* Storage)
{
	if (!IsValid(StorageCache) || !StorageId.IsValid() || !IsValid(Storage))
	{
		LOG_ERROR(LogStorage, TEXT("Save game, storage id or class is invalid"));
		return;
	}

	FString StorageClass = Storage->GetClass()->GetPathName();

	FStorageInstance StorageInstance(StorageClass, StorageId);

	FMemoryWriter Writer(StorageInstance.Data, true);
	FStorageArchive Archive(Writer, false);
	Storage->Serialize(Archive);

	int Index = StorageCache->Instances.IndexOfByPredicate([StorageId, StorageClass](const FStorageInstance& Record) { return Record.StorageId == StorageId && Record.StorageClass == StorageClass; });
	if (StorageCache->Instances.IsValidIndex(Index))
	{
		StorageCache->Instances[Index] = MoveTemp(StorageInstance);
	}
	else
	{
		StorageCache->Instances.Add(MoveTemp(StorageInstance));
	}
}

UObject* UStorageSubsystem::CreateStorageManager(const FStorageDefinition& Definition)
{
	TSubclassOf<UObject> StorageManagerClass = Definition.ManagerClass;
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


void UStorageSubsystem::LoadStorageCache()
{
	StorageCache = Cast<UStorageCache>(UGameplayStatics::LoadGameFromSlot(TEXT("StorageCache"), 0));
	if (!StorageCache)
	{
		StorageCache = Cast<UStorageCache>(UGameplayStatics::CreateSaveGameObject(UStorageCache::StaticClass()));
	}
}

void UStorageSubsystem::UnloadStorageCache()
{
	if (!IsValid(StorageCache))
	{
		LOG_ERROR(LogStorage, TEXT("Save game is invalid"));
		return;
	}

	for (const TPair<FName, TObjectPtr<UObject>>& Kv : ManagerCollection)
	{
		UnloadStorage(Kv.Key);
	}
	ManagerCollection.Empty();

	UGameplayStatics::SaveGameToSlot(StorageCache, TEXT("StorageCache"), 0);
}

void UStorageSubsystem::UnloadStorage(const FName& StorageId)
{
	IStorageManager* StorageManager = Cast<IStorageManager>(GetStorageManager(StorageId));
	if (!StorageManager)
	{
		LOG_ERROR(LogStorage, TEXT("Storage manager is invalid"));
		return;
	}

	StorageManager->OnBeginUnload();

	SerializeStorage(StorageId, StorageManager->GetStorage());

	StorageManager->SetStorage(nullptr);
	StorageManager->OnAfterUnload();
}


bool UStorageSubsystem::ShouldCreateSubsystem(UObject* Object) const
{
	return true;
}

void UStorageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogStorage, TEXT("StorageSubsystem initialized"));

	LoadStorageCache();
}

void UStorageSubsystem::Deinitialize()
{
	UnloadStorageCache();

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


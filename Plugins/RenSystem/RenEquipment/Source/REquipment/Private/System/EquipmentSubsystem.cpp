// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentSubsystem.h"

// Project Headers
#include "Auth/AARemoveEquipmentSlot.h"
#include "Auth/AASetEquipmentSlot.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentStorage.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Core/Interface/AssetInstanceRelation.h"
#include "Interface/StorageManager.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/AssetInstanceRelationSubsystem.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "System/EquipmentStorageManager.h"


void UEquipmentSubsystem::SyncEquipment(const FGuid& OwnerInstanceId) const
{
	return OnSyncEquipment.Broadcast(OwnerInstanceId);
}

UEquipmentStorageManager* UEquipmentSubsystem::GetStorageManager() const
{
	if (!StorageProvider)
	{
		return nullptr;
	}

	FName StorageId = UEquipmentSettings::Get()->StorageId;
	return StorageProvider->GetStorageManager<UEquipmentStorageManager>(StorageId);
}

bool UEquipmentSubsystem::TrySetEquipmentSlot(const FGuid& OwnerInstanceId, const FPrimaryAssetId& OwnerAssetId, const FGuid& EquipmentInstanceId, const FPrimaryAssetId& EquipmentAssetId, const FEquipmentSlotId& SlotId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AuthActionSubsystem not found"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAASetEquipmentSlot* Action = AuthActionSubsystem->CreateAction<UAASetEquipmentSlot>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to create auth action"));
		return false;
	}

	Action->OwnerInstanceId = OwnerInstanceId;
	Action->OwnerAssetId = OwnerAssetId;
	Action->EquipmentInstanceId = EquipmentInstanceId;
	Action->EquipmentAssetId = EquipmentAssetId;
	Action->SlotId = SlotId;
	return Action->StartAction();
}

bool UEquipmentSubsystem::TryRemoveEquipmentSlot(const FGuid& OwnerInstanceId, const FEquipmentSlotId& SlotId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AuthActionSubsystem not found"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAARemoveEquipmentSlot* Action = AuthActionSubsystem->CreateAction<UAARemoveEquipmentSlot>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to create auth action"));
		return false;
	}

	Action->OwnerInstanceId = OwnerInstanceId;
	Action->SlotId = SlotId;
	return Action->StartAction();
}

void UEquipmentSubsystem::HandleStorageLoaded(const FTaskResult& Result)
{
	if (Result.State == ETaskState::Completed)
	{
		const UEquipmentSettings* Settings = UEquipmentSettings::Get();

		const FPrimaryAssetType& EquipmentType = Settings->EquipmentType;
		const FPrimaryAssetType& OwnerType = Settings->OwnerType;

		IAssetInstanceRelation* Relation = Cast<IAssetInstanceRelation>(GetStorageManager());
		UAssetInstanceRelationSubsystem* RelationSubsystem = UAssetInstanceRelationSubsystem::Get(GetGameInstance());

		if (IsValid(RelationSubsystem))
		{
			RelationSubsystem->RegisterRelation(EquipmentType, Relation);
			RelationSubsystem->RegisterRelation(OwnerType, Relation);
		}
	}
}

void UEquipmentSubsystem::HandleOnPreGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (StorageProvider)
	{
		const UEquipmentSettings* Settings = UEquipmentSettings::Get();

		FStorageDefinition Definition;
		Definition.StorageId = Settings->StorageId;
		Definition.StorageClass = Settings->StorageClass;
		Definition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(Definition, FTaskCallback::CreateUObject(this, &UEquipmentSubsystem::HandleStorageLoaded));
	}
}

bool UEquipmentSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UEquipmentSettings::Get()->SubsystemClass;
}

void UEquipmentSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEquipment, TEXT("EquipmentSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UEquipmentSubsystem::HandleOnPreGameInitialized);
}

void UEquipmentSubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogEquipment, TEXT("EquipmentSubsystem deinitialized"));
	Super::Deinitialize();
}

UEquipmentSubsystem* UEquipmentSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UEquipmentSubsystem* UEquipmentSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UEquipmentSubsystem>();
}


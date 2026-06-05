// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/RewardSubsystem.h"

// Project Headers
#include "Asset/RewardAsset.h"
#include "Auth/AAGiveReward.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/RewardSettings.h"
#include "Storage/RewardStorage.h"
#include "Storage/RewardStorageManager.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Subsystem/BroadcastSubsystem.h"


URewardStorageManager* URewardSubsystem::GetStorageManager() const
{
	if (!StorageProvider)
	{
		return nullptr;
	}
	const URewardSettings* Settings = URewardSettings::Get();
	return StorageProvider->GetStorageManager<URewardStorageManager>(Settings->StorageId);
}

void URewardSubsystem::HandleRewardInvoke(const FInstancedStruct& Payload)
{
	const FPrimaryAssetId* AssetId = Payload.GetPtr<FPrimaryAssetId>();
	if (!AssetId || !IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AssetId, AuthActionSubsystem is invalid"));
		return;
	}

	UAAGiveReward* Action = AuthActionSubsystem->CreateAction<UAAGiveReward>(FGuid::NewGuid());
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to create give reward action"));
		return;
	}

	Action->RewardAssetId = *AssetId;
	Action->StartAction();
}

void URewardSubsystem::HandleStorageLoaded(const FTaskResult& Result)
{
	const URewardSettings* Settings = URewardSettings::Get();

	AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	BroadcastSubsystem = UBroadcastSubsystem::Get(GetGameInstance());
	if (IsValid(BroadcastSubsystem))
	{
		BroadcastSubsystem->RegisterBroadcast(Settings->TagEvent_RewardInvoke, FBroadcastDelegate::FDelegate::CreateUObject(this, &URewardSubsystem::HandleRewardInvoke));
	}
}

void URewardSubsystem::HandleGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (StorageProvider)
	{
		const URewardSettings* Settings = URewardSettings::Get();

		FStorageDefinition StorageDefinition;
		StorageDefinition.StorageId = Settings->StorageId;
		StorageDefinition.StorageClass = Settings->StorageClass;
		StorageDefinition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(StorageDefinition, FTaskCallback::CreateUObject(this, &URewardSubsystem::HandleStorageLoaded));
	}
}

bool URewardSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const URewardSettings* Settings = URewardSettings::Get();
	return GetClass() == Settings->SubsystemClass;
}

void URewardSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("RewardSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &URewardSubsystem::HandleGameInitialized);
}

void URewardSubsystem::Deinitialize()
{
	if (IsValid(BroadcastSubsystem))
	{
		const URewardSettings* Settings = URewardSettings::Get();
		BroadcastSubsystem->UnregisterBroadcast(Settings->TagEvent_RewardInvoke, this);
	}

	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogTemp, TEXT("RewardSubsystem deinitialized"));
	Super::Deinitialize();
}

URewardSubsystem* URewardSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

URewardSubsystem* URewardSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<URewardSubsystem>();
}


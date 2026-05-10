// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/PartySubsystem.h"

// Project Headers
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorage.h"
#include "Storage/PartyStorageManager.h"
#include "Util/SubsystemUtil.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Auth/AASetPartyCharacter.h"
#include "Auth/AARemovePartyCharacter.h"


UPartyStorageManager* UPartySubsystem::GetStorageManager()
{
	if (!IsValid(StorageManager))
	{
		if (!StorageProvider)
		{
			return nullptr;
		}

		FName StorageId = UPartySettings::Get()->StorageId;
		StorageManager = StorageProvider->GetStorageManager<UPartyStorageManager>(StorageId);
	}
	return StorageManager;
}

void UPartySubsystem::SyncParty()
{
	OnSyncParty.Broadcast();
}

bool UPartySubsystem::TrySetPartyCharacter(int Slot, const FPrimaryAssetId& AssetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AuthActionSubsystem not found"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAASetPartyCharacter* Action = AuthActionSubsystem->CreateAction<UAASetPartyCharacter>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to create auth action"));
		return false;
	}

	Action->CharacterSlot = Slot;
	Action->CharacterAssetId = AssetId;
	return Action->StartAction();
}

bool UPartySubsystem::TryRemovePartyCharacter(int Slot)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AuthActionSubsystem not found"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAARemovePartyCharacter* Action = AuthActionSubsystem->CreateAction<UAARemovePartyCharacter>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to create auth action"));
		return false;
	}

	Action->CharacterSlot = Slot;

	return Action->StartAction();
}


void UPartySubsystem::HandlePreGameInitialized()
{
	StorageProvider = SubsystemUtil::GetSubsystemInterface<IStorageProvider>(GetGameInstance());
	if (StorageProvider)
	{
		const UPartySettings* Settings = UPartySettings::Get();

		FStorageDefinition Definition;
		Definition.StorageId = Settings->StorageId;
		Definition.StorageClass = Settings->StorageClass;
		Definition.ManagerClass = Settings->StorageManagerClass;

		StorageProvider->LoadStorage(Definition, FTaskCallback());
	}
}

bool UPartySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UPartySettings::Get()->SubsystemClass;
}

void UPartySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCharacterParty, TEXT("PartySubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UPartySubsystem::HandlePreGameInitialized);
}

void UPartySubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;
	StorageManager = nullptr;

	LOG_WARNING(LogCharacterParty, TEXT("PartySubsystem deinitialized"));
	Super::Deinitialize();
}

UPartySubsystem* UPartySubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UPartySubsystem* UPartySubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UPartySubsystem>();
}


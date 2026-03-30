// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/PartySubsystem.h"

// Project Headers
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorage.h"
#include "Util/SubsystemUtil.h"


UPartyStorage* UPartySubsystem::GetPartyStorage()
{
	if (!IsValid(_CachedStorage))
	{
		IStorageProvider* StorageInterface = StorageProvider.Get();
		if (!StorageInterface)
		{
			return nullptr;
		}

		FName StorageId = UPartySettings::Get()->StorageId;
		UStorage* Storage = StorageInterface->GetStorage(StorageId);

		_CachedStorage = Cast<UPartyStorage>(Storage);
	}
	return _CachedStorage;
}

void UPartySubsystem::SyncParty()
{
	OnSyncParty.Broadcast();
}

void UPartySubsystem::OnPreGameInitialized()
{
	IStorageProvider* StorageInterface = SubsystemUtil::GetSubsystemInterface<IStorageProvider>(GetGameInstance());
	if (StorageInterface)
	{
		StorageInterface->LoadStorageFromSettings(UPartySettings::Get());
		StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
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

	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UPartySubsystem::OnPreGameInitialized);
}

void UPartySubsystem::Deinitialize()
{
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);
	StorageProvider.Reset();
	_CachedStorage = nullptr;

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


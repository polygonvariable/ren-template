// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "GameFramework/Character.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Actor/AvatarCharacter.h"
#include "Core/PartySettings.h"
#include "Data/CharacterAsset.h"
#include "Library/AssetManagerUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/PartyStorageManager.h"
#include "System/PartySubsystem.h"


UPartyManagerComponent::UPartyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent::BeginPlay()
{
	AssetManager = UAssetManager::GetIfInitialized();

	if (SourceType == EDataSource::Runtime)
	{
		PartySubsystem = UPartySubsystem::Get(GetWorld());
		if (IsValid(PartySubsystem))
		{
			PartySubsystem->OnSyncParty.AddUObject(this, &UPartyManagerComponent::SpawnParty);
			StorageManager = PartySubsystem->GetStorageManager();
		}
	}

	Super::BeginPlay();
}

void UPartyManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FAssetManagerUtil::CancelHandle(_SpawnHandle);

	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnSyncParty.RemoveAll(this);
	}

	AssetManager = nullptr;
	PartySubsystem = nullptr;
	StorageManager = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UPartyManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPartyManagerComponent::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	CharacterAssetIds.Empty();
	if (SourceType == EDataSource::Static)
	{
		for (const FCharacterInitializationData& Data : CharacterData)
		{
			CharacterAssetIds.Add(Data.AssetId);
		}
	}
	else
	{
		CharacterData.Empty();
	}
#endif
}

void UPartyManagerComponent::SpawnParty()
{
	RefreshPartyOrder();
	RefreshSpawnData();
	CleanupPartyCharacters();

	if (CharacterAssetIds.Num() == 0)
	{
		return;
	}

	FAssetManagerUtil::CancelHandle(_SpawnHandle);

	const UPartySettings* Settings = UPartySettings::Get();
	const TArray<FName>& AssetBundles = Settings->CharacterBundles;

	_SpawnHandle = AssetManager->LoadPrimaryAssets(CharacterAssetIds, AssetBundles, FStreamableDelegate::CreateUObject(this, &UPartyManagerComponent::SpawnPartyCharacters));
}

void UPartyManagerComponent::SpawnPartyCharacters()
{
	FAssetManagerUtil::ReleaseHandle(_SpawnHandle);

	for (const FCharacterInitializationData& Data : CharacterData)
	{
		SpawnCharacter(Data.AssetId, Data);
	}
	PossessAliveCharacter();
}

void UPartyManagerComponent::SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterInitializationData& Data)
{
#if WITH_EDITOR
	const uint64 StartCycles = FPlatformTime::Cycles64();
#endif

	const UCharacterAsset* Asset = AssetManager->GetPrimaryAssetObject<UCharacterAsset>(AssetId);
	if (!IsValid(Asset) || PartyCharacters.Contains(AssetId))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Character asset is invalid or already spawned"));
		return;
	}

	UClass* CharacterClass = Asset->CharacterClass.Get();
	if (!CharacterClass)
	{
		LOG_ERROR(LogCharacterParty, TEXT("CharacterClass is invalid"));
		return;
	}

	FTransform SpawnTransform;
	AAvatarCharacter* Character = GetWorld()->SpawnActorDeferred<AAvatarCharacter>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned character is invalid"));
		return;
	}

	Character->CharacterAsset = Asset;
	Character->CharacterData = Data;
	Character->SourceType = SourceType;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterCharacter(AssetId, Character);

#if WITH_EDITOR
	const uint64 EndCycles = FPlatformTime::Cycles64();
	const double TimeElapsed = FPlatformTime::ToMilliseconds64(EndCycles - StartCycles);
	UE_LOG(LogTemp, Log, TEXT("Party creation took: %.4f ms"), TimeElapsed);
#endif
}

void UPartyManagerComponent::RegisterCharacter(const FPrimaryAssetId& AssetId, AAvatarCharacter* Character)
{
	PartyCharacters.Add(AssetId, Character);
}

void UPartyManagerComponent::UnregisterCharacter(const FPrimaryAssetId& AssetId)
{
	AAvatarCharacter* Character = PartyCharacters.FindAndRemoveChecked(AssetId);
	if (IsValid(Character))
	{
		Character->DeinitializeCharacter();
		Character->Destroy();
	}
}

void UPartyManagerComponent::RefreshPartyOrder()
{
	if (IsValid(StorageManager) && SourceType == EDataSource::Runtime)
	{
		StorageManager->GetAllCharacters(CharacterAssetIds);
	}
}

void UPartyManagerComponent::RefreshSpawnData()
{
	if (SourceType == EDataSource::Runtime)
	{
		CharacterData.Empty();
		for (const FPrimaryAssetId& AssetId : CharacterAssetIds)
		{
			FCharacterInitializationData Data;
			Data.AssetId = AssetId;

			CharacterData.Add(Data);
		}
	}
}

void UPartyManagerComponent::CleanupPartyCharacters()
{
	TArray<FPrimaryAssetId> CurrentIds;
	PartyCharacters.GetKeys(CurrentIds);
	
	for (const FPrimaryAssetId& AssetId : CurrentIds)
	{
		if (!CharacterAssetIds.Contains(AssetId))
		{
			UnregisterCharacter(AssetId);
		}
	}
}

void UPartyManagerComponent::PossessAliveCharacter()
{
	APlayerController* PC = GetController();
	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character) && IsValid(PC))
	{
		PC->Possess(Character);
	}
}

ACharacter* UPartyManagerComponent::GetAliveCharacter() const
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<AAvatarCharacter>>& Kv : PartyCharacters)
	{
		AAvatarCharacter* Actor = Kv.Value.Get();
		if (IsValid(Actor) && Actor->IsAlive())
		{
			return Actor;
		}
	}
	return nullptr;
}

APlayerController* UPartyManagerComponent::GetController() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}


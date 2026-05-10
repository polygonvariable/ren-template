// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent.h"

// Engine Headers
#include "GameFramework/Character.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorageManager.h"
#include "Subsystem/PartySubsystem.h"


UPartyManagerComponent::UPartyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent::BeginPlay()
{
	AssetManager = URAssetManager::Get();

	if (SourceType == EAssetQuerySource::Instance)
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
	if (SourceType == EAssetQuerySource::Asset)
	{
		for (const FCharacterData& Data : CharacterSpawnData)
		{
			CharacterAssetIds.Add(Data.AssetId);
		}
	}
	else
	{
		CharacterSpawnData.Empty();
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

	AssetManager->CancelFetch(_SpawnId);

	_SpawnId = FGuid::NewGuid();

	const TArray<FName>& AssetBundles = UPartySettings::Get()->CharacterBundles;

	TWeakObjectPtr<UPartyManagerComponent> WeakThis(this);
	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_SpawnId, CharacterAssetIds, AssetBundles, false);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
		{
			UPartyManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsCompleted())
			{
				This->SpawnPartyCharacters();
			}
		}
	);
}

void UPartyManagerComponent::SpawnPartyCharacters()
{
	for (const FCharacterData& Data : CharacterSpawnData)
	{
		SpawnCharacter(Data.AssetId, Data);
	}

	PossessAliveCharacter();
}

void UPartyManagerComponent::SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterData& Data)
{
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
	ACharacterBase* Character = GetWorld()->SpawnActorDeferred<ACharacterBase>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned character is invalid"));
		return;
	}

	Character->CharacterAsset = Asset;
	Character->CharacterData = Data;
	Character->InitializeCharacter();
	Character->FinishSpawning(SpawnTransform);

	RegisterCharacter(AssetId, Character);
}

void UPartyManagerComponent::RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character)
{
	PartyCharacters.Add(AssetId, Character);
}

void UPartyManagerComponent::UnregisterCharacter(const FPrimaryAssetId& AssetId)
{
	ACharacterBase* Character = PartyCharacters.FindAndRemoveChecked(AssetId);
	if (IsValid(Character))
	{
		Character->DeinitializeCharacter();
		Character->Destroy();
	}
}

void UPartyManagerComponent::RefreshPartyOrder()
{
	if (IsValid(StorageManager) && SourceType == EAssetQuerySource::Instance)
	{
		StorageManager->GetAllCharacters(CharacterAssetIds);
	}
}

void UPartyManagerComponent::RefreshSpawnData()
{
	if (SourceType == EAssetQuerySource::Instance)
	{
		CharacterSpawnData.Empty();
		for (const FPrimaryAssetId& AssetId : CharacterAssetIds)
		{
			FCharacterData Data;
			Data.AssetId = AssetId;
			Data.SourceType = EAssetQuerySource::Instance;

			CharacterSpawnData.Add(Data);
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
	for (const TPair<FPrimaryAssetId, TObjectPtr<ACharacterBase>>& Kv : PartyCharacters)
	{
		ACharacterBase* Actor = Kv.Value.Get();
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


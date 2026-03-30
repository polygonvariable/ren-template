// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/PartyManagerComponent.h"

// Engine Headers
#include "GameFramework/Character.h"

// Project Headers
#include "Actor/RCharacterBase.h"
#include "Asset/CharacterAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Manager/RAssetManager.inl"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorage.h"
#include "Subsystem/PartySubsystem.h"


UPartyManagerComponent::UPartyManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	SetIsReplicatedByDefault(true);
}

void UPartyManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPartyManagerComponent::BeginPlay()
{
	AssetManager = Cast<URAssetManager>(UAssetManager::GetIfInitialized());

	PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnSyncParty.AddUObject(this, &UPartyManagerComponent::SpawnParty);
		PartyStorage = PartySubsystem->GetPartyStorage();
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
	PartyStorage = nullptr;

	Super::EndPlay(EndPlayReason);
}

void UPartyManagerComponent::SwitchCharacter(int Index)
{
	if (!PartyOrder.IsValidIndex(Index))
	{
		return;
	}

	TObjectPtr<ARCharacterBase>* FoundCharacter = PartyCharacters.Find(PartyOrder[Index]);
	if (!FoundCharacter)
	{
		return;
	}

	ARCharacterBase* Character = FoundCharacter->Get();
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(Character) || !Character->IsAlive() || !IsValid(PC))
	{
		return;
	}

	PC->Possess(Character);
}

void UPartyManagerComponent::SpawnParty()
{
	if (!IsValid(AssetManager) || !IsValid(PartyStorage))
	{
		LOG_ERROR(LogCharacterParty, TEXT("AssetManager, Party Storage is invalid"));
		return;
	}

	PartyStorage->GetAllCharacters(PartyOrder);

	TArray<FPrimaryAssetId> CurrentIds;
	PartyCharacters.GetKeys(CurrentIds);

	for (const FPrimaryAssetId& AssetId : CurrentIds)
	{
		if (!PartyOrder.Contains(AssetId))
		{
			ARCharacterBase* Character = PartyCharacters.FindAndRemoveChecked(AssetId);
			if (Character)
			{
				RemoveCharacter(Character);
			}
		}
	}

	TArray<FPrimaryAssetId> SpawnList;
	for (const FPrimaryAssetId& AssetId : PartyOrder)
	{
		if (!PartyCharacters.Contains(AssetId))
		{
			SpawnList.Add(AssetId);
		}
	}

	if (SpawnList.Num() == 0)
	{
		PossessAliveCharacter();
		return;
	}

	const UPartySettings* Settings = UPartySettings::Get();

	AssetManager->CancelFetch(_PartySpawnId);

	_PartySpawnId = FGuid::NewGuid();
	_PartySpawnLocation = PartyStorage->GetPartyLocation(*GetWorld()->GetMapName());

	const TArray<FName>& AssetBundles = Settings->CharacterBundles;
	//AssetBundles.Add(TEXT("Character"));
	//AssetBundles.Add(TEXT("Ability"));

	TFuture<FLatentLoadedAssets<UCharacterAsset>> Future = AssetManager->FetchPrimaryAssets<UCharacterAsset>(_PartySpawnId, SpawnList, AssetBundles);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogCharacterParty, TEXT("Failed to initialize load character assets"));
		return;
	}

	TWeakObjectPtr<UPartyManagerComponent> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAssets<UCharacterAsset>& Result)
		{
			UPartyManagerComponent* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->SpawnParty_Internal(Result.Get());
			}
		}
	);
}

void UPartyManagerComponent::SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets)
{
	UWorld* World = GetWorld();

	for (const UCharacterAsset* Asset : Assets)
	{
		if (IsValid(Asset))
		{
			SpawnCharacter(World, Asset);
		}
	}

	PossessAliveCharacter();
}


void UPartyManagerComponent::SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset)
{
	UClass* CharacterClass = CharacterAsset->CharacterClass.Get();
	if (!CharacterClass)
	{
		LOG_ERROR(LogCharacterParty, TEXT("CharacterClass is invalid"));
		return;
	}

	FTransform SpawnTransform;
	ARCharacterBase* Character = World->SpawnActorDeferred<ARCharacterBase>(CharacterClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("Spawned character is invalid"));
		return;
	}

	Character->InitializeCharacter(CharacterAsset);
	Character->FinishSpawning(SpawnTransform);

	RegisterCharacter(Character);

	PartyCharacters.Add(CharacterAsset->GetPrimaryAssetId(), Character);
}

void UPartyManagerComponent::RemoveCharacter(ARCharacterBase* Character)
{
	UnregisterCharacter(Character);

	Character->Destroy();
}


void UPartyManagerComponent::RegisterCharacter(ARCharacterBase* Character)
{
	Character->OnCharacterDied.AddUObject(this, &UPartyManagerComponent::HandleOnCharacterDied);
}

void UPartyManagerComponent::UnregisterCharacter(ARCharacterBase* Character)
{
	Character->OnCharacterDied.RemoveAll(this);
	Character->DeinitializeCharacter();
	Character->Destroy();
}


void UPartyManagerComponent::HandleOnCharacterDied()
{
	ACharacter* Character = GetAliveCharacter();
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC) || !IsValid(Character))
	{
		LOG_ERROR(LogCharacterParty, TEXT("PC, Character is invalid"));
		return;
	}

	PC->Possess(Character);
}


void UPartyManagerComponent::ClearParty()
{
	APlayerController* PC = GetControllerWithAuthority();
	if (!IsValid(PC))
	{
		LOG_ERROR(LogCharacterParty, TEXT("PC is invalid"));
		return;
	}

	PC->UnPossess();
}

void UPartyManagerComponent::PossessAliveCharacter()
{
	APlayerController* PC = GetControllerWithAuthority();
	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character) && IsValid(PC))
	{
		PC->Possess(Character);
	}
}

ACharacter* UPartyManagerComponent::GetAliveCharacter() const
{
	for (const TPair<FPrimaryAssetId, TObjectPtr<ARCharacterBase>>& Kv : PartyCharacters)
	{
		ARCharacterBase* Actor = Kv.Value.Get();
		if (IsValid(Actor) && Actor->IsAlive())
		{
			return Actor;
		}
	}
	return nullptr;
}


APlayerController* UPartyManagerComponent::GetControllerWithAuthority() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}
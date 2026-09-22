// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/AvatarSpawnerComponent.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "GameFramework/Character.h"

// Project Headers
#include "Actor/AvatarCharacter.h"
#include "System/PartyStorageManager.h"
#include "System/PartySubsystem.h"


void UAvatarSpawnerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (SourceType == EDataSource::Runtime)
	{
		PartySubsystem = UPartySubsystem::Get(GetWorld());
		if (IsValid(PartySubsystem))
		{
			PartySubsystem->OnPartyUpdated.AddUObject(this, &UAvatarSpawnerComponent::HandleOnCharactersUpdated);
			StorageManager = PartySubsystem->GetStorageManager();
		}
	}
}

void UAvatarSpawnerComponent::UninitializeComponent()
{
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnPartyUpdated.RemoveAll(this);
	}

	PartySubsystem = nullptr;
	StorageManager = nullptr;

	Super::UninitializeComponent();
}


APlayerController* UAvatarSpawnerComponent::GetController() const
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	const ENetRole OwnerRole = GetOwnerRole();
	if (OwnerRole != ENetRole::ROLE_Authority || !IsValid(PC))
	{
		return nullptr;
	}
	return PC;
}

void UAvatarSpawnerComponent::PossessCharacter()
{
	APlayerController* PC = GetController();
	ACharacter* Character = GetAliveCharacter();
	if (IsValid(Character) && IsValid(PC))
	{
		PC->Possess(Character);
	}
}

void UAvatarSpawnerComponent::UnPossessCharacter()
{
	APlayerController* PC = GetController();
	if (IsValid(PC))
	{
		PC->UnPossess();
	}
}


void UAvatarSpawnerComponent::HandleOnCharactersUpdated()
{
	RemoveCharacters();
	CreateCharacters();
}


void UAvatarSpawnerComponent::OnSpawnStarted()
{
	UnPossessCharacter();
}

void UAvatarSpawnerComponent::OnSpawnFinished()
{
	PossessCharacter();
}


void UAvatarSpawnerComponent::GetCharacterData(TArray<FCharacterInitializationData>& OutData)
{
	if (SourceType == EDataSource::Static)
	{
		OutData = CharacterData;
	}
	else
	{
		if (IsValid(StorageManager))
		{
			TArray<FPrimaryAssetId> AssetIds;
			StorageManager->GetAllCharacters(AssetIds);

			OutData.Empty();
			for (const FPrimaryAssetId& AssetId : AssetIds)
			{
				OutData.Add(FCharacterInitializationData(AssetId));
			}
		}
	}
}

void UAvatarSpawnerComponent::PreRegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character)
{
	AAvatarCharacter* Avatar = Cast<AAvatarCharacter>(Character);
	if (IsValid(Avatar))
	{
		Avatar->SourceType = SourceType;
	}
}


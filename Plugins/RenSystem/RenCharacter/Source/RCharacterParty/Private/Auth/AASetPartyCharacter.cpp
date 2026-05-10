// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AASetPartyCharacter.h"

// Project Headers
#include "Delegate/GameUIDelegate.h"
#include "Library/CharacterPrimaryAsset.h"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorageManager.h"
#include "Subsystem/PartySubsystem.h"


void UAASetPartyCharacter::OnStarted()
{
	FGameUIDelegate::OnUIActionStarted.Broadcast();
	PartySubsystem = UPartySubsystem::Get(GetWorld());

	Step_AssignCharacter();
}

void UAASetPartyCharacter::OnCompleted(bool bSuccess)
{
	FGameUIDelegate::OnUIActionCompleted.Broadcast();
}

void UAASetPartyCharacter::OnCleanup()
{
	PartySubsystem = nullptr;
	CharacterSlot = 0;
	CharacterAssetId = FPrimaryAssetId();
}

void UAASetPartyCharacter::Step_AssignCharacter()
{
	const UPartySettings* Settings = UPartySettings::Get();
	if (!FCharacterPrimaryAsset::IsValid(CharacterAssetId))
	{
		Fail(TEXT("Invalid Asset Id"));
		return;
	}

	UPartyStorageManager* StorageManager = PartySubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("Storage Manager is invalid"));
		return;
	}

	if (!StorageManager->SetCharacterAtSlot(CharacterSlot, CharacterAssetId))
	{
		Fail(TEXT("Failed to set character at slot"));
		return;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &UAASetPartyCharacter::Step_Finalize, 0.5f, false);
}

void UAASetPartyCharacter::Step_Finalize()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(TimerHandle);

	Success();
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Auth/AARemovePartyCharacter.h"

// Project Headers
#include "Delegate/GameUIDelegate.h"
#include "Library/CharacterPrimaryAsset.h"
#include "Settings/PartySettings.h"
#include "Storage/PartyStorageManager.h"
#include "Subsystem/PartySubsystem.h"


void UAARemovePartyCharacter::OnStarted()
{
	FGameUIDelegate::OnUIActionStarted.Broadcast();
	PartySubsystem = UPartySubsystem::Get(GetWorld());

	Step_RemoveCharacter();
}

void UAARemovePartyCharacter::OnCompleted(bool bSuccess)
{
	FGameUIDelegate::OnUIActionCompleted.Broadcast();
}

void UAARemovePartyCharacter::OnCleanup()
{
	PartySubsystem = nullptr;
	CharacterSlot = 0;
}

void UAARemovePartyCharacter::Step_RemoveCharacter()
{
	UPartyStorageManager* StorageManager = PartySubsystem->GetStorageManager();
	if (!IsValid(StorageManager))
	{
		Fail(TEXT("Storage Manager is invalid"));
		return;
	}

	if (!StorageManager->RemoveCharacterFromSlot(CharacterSlot))
	{
		Fail(TEXT("Failed to remove character from slot"));
		return;
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &UAARemovePartyCharacter::Step_Finalize, 0.5f, false);
}

void UAARemovePartyCharacter::Step_Finalize()
{
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(TimerHandle);

	Success();
}


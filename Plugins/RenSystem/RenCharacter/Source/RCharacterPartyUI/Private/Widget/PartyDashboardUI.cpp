// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/PartyDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Subsystem/PartySubsystem.h"
#include "Widget/AssetCollectionUI.h"


void UPartyDashboardUI::InitializeDetail()
{
	AvatarCollection->PrimarySourceId = PrimarySourceId;
	AvatarCollection->InitializeCollection();
	AvatarCollection->DisplayEntries();
}

void UPartyDashboardUI::SyncParty()
{
	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->SyncParty();
	}
}

void UPartyDashboardUI::NativeConstruct()
{
	Super::NativeConstruct();

	SyncButton->OnClicked.AddDynamic(this, &UPartyDashboardUI::SyncParty);
}

void UPartyDashboardUI::NativeDestruct()
{
	SyncButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}


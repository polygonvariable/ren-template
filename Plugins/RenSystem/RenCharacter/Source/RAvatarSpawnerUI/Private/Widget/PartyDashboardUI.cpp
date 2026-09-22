// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/PartyDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Delegate/GameUIDelegate.h"
#include "System/PartySubsystem.h"
#include "Widget/AssetCollectionUI.h"


void UPartyDashboardUI::InitializeDetail()
{
	AvatarCollection->PrimarySourceId = PrimarySourceId;
	AvatarCollection->InitializeCollection();
	AvatarCollection->DisplayEntries();
}

void UPartyDashboardUI::ApplyParty()
{
	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		PartySubsystem->OnPartyUpdated.Broadcast();
	}
}

void UPartyDashboardUI::NativeConstruct()
{
	Super::NativeConstruct();

	FGameUIDelegate::OnUIActionStarted.AddUObject(this, &UPartyDashboardUI::LockControls);
	FGameUIDelegate::OnUIActionCompleted.AddUObject(this, &UPartyDashboardUI::UnlockControls);

	ApplyButton->OnClicked.AddDynamic(this, &UPartyDashboardUI::ApplyParty);
}

void UPartyDashboardUI::NativeDestruct()
{
	FGameUIDelegate::OnUIActionStarted.RemoveAll(this);
	FGameUIDelegate::OnUIActionCompleted.RemoveAll(this);

	ApplyButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}


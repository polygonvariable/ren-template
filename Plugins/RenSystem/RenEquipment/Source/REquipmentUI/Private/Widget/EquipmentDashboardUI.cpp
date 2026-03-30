// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Subsystem/EquipmentSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/EquipmentSlotCollectionUI.h"


void UEquipmentDashboardUI::InitializeDetail()
{
	OwnerDetail->InitializeDetail();

	EquipmentCollection->InitializeCollection();
	EquipmentCollection->DisplayEntries();
}

void UEquipmentDashboardUI::SyncEquipment()
{
	if (IsValid(EquipmentSubsystem))
	{
		EquipmentSubsystem->SyncEquipment(OwnerId);
	}
}

void UEquipmentDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	OwnerDetail->InitializeAssetDetail(Asset);
}

void UEquipmentDashboardUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	if (!IsValid(Entry))
	{
		return;
	}

	OwnerId = Entry->GetAssetInstanceId();

	OwnerDetail->InitializeEntryDetail(Entry);
	SlotCollection->InitializeEntryDetail(Entry);
}

void UEquipmentDashboardUI::NativeConstruct()
{
	SyncButton->OnClicked.AddDynamic(this, &UEquipmentDashboardUI::SyncEquipment);
	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());

	Super::NativeConstruct();
}

void UEquipmentDashboardUI::NativeDestruct()
{
	SyncButton->OnClicked.RemoveAll(this);
	EquipmentSubsystem = nullptr;

	Super::NativeDestruct();
}


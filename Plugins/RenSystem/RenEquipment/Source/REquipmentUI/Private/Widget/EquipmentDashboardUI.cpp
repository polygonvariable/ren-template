// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Definition/AssetFilterProperty.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Storage/EquipmentStorage.h"
#include "Subsystem/EquipmentSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/EquipmentSlotCollectionUI.h"


void UEquipmentDashboardUI::InitializeDetail()
{
	OwnerDetail->InitializeDetail();
	EquipmentCollection->InitializeCollection();
}

void UEquipmentDashboardUI::SyncEquipment()
{
	if (IsValid(EquipmentSubsystem))
	{
		EquipmentSubsystem->SyncEquipment(OwnerInstanceId);
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

	OwnerInstanceId = Entry->GetAssetInstanceId();

	OwnerDetail->InitializeEntryDetail(Entry);
	SlotCollection->InitializeEntryDetail(Entry);

	if (IsValid(EquipmentStorage))
	{
		TArray<FGuid> EquipmentIds;
		EquipmentStorage->GetNonOwnedEquipmentIds(OwnerInstanceId, EquipmentIds);

		UFilterCriterion_Guid* AssetFilter = EquipmentCollection->GetCriterionByName<UFilterCriterion_Guid>(FAssetFilterProperty::InstanceId);
		if (IsValid(AssetFilter))
		{
			AssetFilter->Included.Empty();
			AssetFilter->Included.Append(EquipmentIds);
		}

		EquipmentCollection->DisplayEntries();
	}
}

void UEquipmentDashboardUI::NativeConstruct()
{
	SyncButton->OnClicked.AddDynamic(this, &UEquipmentDashboardUI::SyncEquipment);
	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
	EquipmentStorage = EquipmentSubsystem->GetEquipmentStorage();

	Super::NativeConstruct();
}

void UEquipmentDashboardUI::NativeDestruct()
{
	SyncButton->OnClicked.RemoveAll(this);
	EquipmentSubsystem = nullptr;
	EquipmentStorage = nullptr;

	Super::NativeDestruct();
}


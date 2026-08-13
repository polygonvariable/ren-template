// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Core/Type/AssetFilterProperty.h"
#include "Delegate/GameUIDelegate.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "System/EquipmentStorageManager.h"
#include "System/EquipmentSubsystem.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/AssetDetailUI.h"
#include "Widget/AssetEntry.h"
#include "Widget/EquipmentSlotCollectionUI.h"


void UEquipmentDashboardUI::InitializeDetail()
{
	OwnerDetail->InitializeDetail();
	EquipmentCollection->InitializeCollection();
}

void UEquipmentDashboardUI::RefreshDetail()
{
	EquipmentCollection->RefreshEntries();
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

	if (IsValid(StorageManager))
	{
		TArray<FGuid> EquipmentIds;
		StorageManager->GetNonOwnedEquipmentIds(OwnerInstanceId, EquipmentIds);

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
	FGameUIDelegate::OnUIActionStarted.AddUObject(this, &UEquipmentDashboardUI::LockControls);
	FGameUIDelegate::OnUIActionCompleted.AddUObject(this, &UEquipmentDashboardUI::UnlockControls);

	EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
	StorageManager = EquipmentSubsystem->GetStorageManager();
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UEquipmentDashboardUI::RefreshDetail);
	}

	Super::NativeConstruct();
}

void UEquipmentDashboardUI::NativeDestruct()
{
	SyncButton->OnClicked.RemoveAll(this);
	FGameUIDelegate::OnUIActionStarted.RemoveAll(this);
	FGameUIDelegate::OnUIActionCompleted.RemoveAll(this);

	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;
	EquipmentSubsystem = nullptr;

	Super::NativeDestruct();
}


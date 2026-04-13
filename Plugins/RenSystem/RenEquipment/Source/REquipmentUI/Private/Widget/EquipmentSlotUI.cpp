// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentSlotUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Storage/EquipmentStorage.h"
#include "Subsystem/EquipmentSubsystem.h"
#include "Widget/AssetEntry.h"
#include "Widget/Drag/AssetDragOperation.h"


void UEquipmentSlotUI::ResetDetail()
{
	AssetDisplayName->SetText(EmptyText);
	AssetIcon->SetBrushFromSoftTexture(EmptyIcon);
}

void UEquipmentSlotUI::RefreshDetail()
{
	if (!IsValid(EquipmentStorage))
	{
		ResetDetail();
		return;
	}

	FPrimaryAssetId EquipmentAssetId;
	if (!EquipmentStorage->GetEquipmentAtSlot(OwnerInstanceId, SlotTag, EquipmentAssetId))
	{
		ResetDetail();
		return;
	}

	InitializeAssetById(EquipmentAssetId);
}

void UEquipmentSlotUI::ClearSlot()
{
	if (IsValid(EquipmentStorage))
	{
		EquipmentStorage->RemoveEquipmentFromSlot(OwnerInstanceId, SlotTag);
	}
}

void UEquipmentSlotUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		ResetDetail();
		return;
	}

	AssetDisplayName->SetText(Asset->DisplayName);
	AssetIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UEquipmentSlotUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	if (!IsValid(Entry))
	{
		return;
	}

	OwnerAssetId = Entry->AssetId;
	OwnerInstanceId = Entry->GetAssetInstanceId();

	RefreshDetail();
}

void UEquipmentSlotUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (bAllowEdit)
	{
		ClearButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ClearButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEquipmentSlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAllowEdit && IsValid(ClearButton))
	{
		ClearButton->OnClicked.AddDynamic(this, &UEquipmentSlotUI::ClearSlot);
	}

	UEquipmentSubsystem* EquipmentSubsystem = UEquipmentSubsystem::Get(GetWorld());
	if (IsValid(EquipmentSubsystem))
	{
		EquipmentStorage = EquipmentSubsystem->GetEquipmentStorage();
		if (IsValid(EquipmentStorage))
		{
			EquipmentStorage->OnStorageUpdated.AddUObject(this, &UEquipmentSlotUI::RefreshDetail);
		}
	}
}

void UEquipmentSlotUI::NativeDestruct()
{
	if (IsValid(ClearButton))
	{
		ClearButton->OnClicked.Clear();
	}

	if (IsValid(EquipmentStorage))
	{
		EquipmentStorage->OnStorageUpdated.RemoveAll(this);
	}
	EquipmentStorage = nullptr;

	Super::NativeDestruct();
}

bool UEquipmentSlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!bAllowEdit || !IsValid(EquipmentStorage))
	{
		return false;
	}

	const UAssetDragOperation* DragOperation = Cast<UAssetDragOperation>(InOperation);
	if (!IsValid(DragOperation))
	{
		return false;
	}

	const FGuid& EquipmentInstanceId = DragOperation->AssetInstanceId;
	const FPrimaryAssetId& EquipmentAssetId = DragOperation->AssetId;
	if (!EquipmentInstanceId.IsValid() || !EquipmentAssetId.IsValid())
	{
		return false;
	}
	
	return EquipmentStorage->SetEquipmentAtSlot(OwnerInstanceId, OwnerAssetId, SlotTag, EquipmentInstanceId, EquipmentAssetId);
}


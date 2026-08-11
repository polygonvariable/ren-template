// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/PartySlotUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "System/PartyStorageManager.h"
#include "System/PartySubsystem.h"
#include "Widget/Drag/AssetDragOperation.h"


void UPartySlotUI::ResetDetail()
{
	AssetDisplayName->SetText(EmptyText);
	AssetIcon->SetBrushFromSoftTexture(EmptyIcon);

	if (bHideOnEmpty)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPartySlotUI::RefreshDetail()
{
	FPrimaryAssetId AssetId;
	if (!IsValid(StorageManager) || !StorageManager->GetCharacterAtSlot(CharacterSlot, AssetId))
	{
		ResetDetail();
		return;
	}
	
	InitializeAssetById(AssetId);
}

void UPartySlotUI::ClearSlot()
{
	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (bAllowEdit && IsValid(PartySubsystem))
	{
		PartySubsystem->TryRemovePartyCharacter(CharacterSlot);
	}
}

void UPartySlotUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	AssetDisplayName->SetText(Asset->DisplayName);
	AssetIcon->SetBrushFromSoftTexture(Asset->Icon);

	if (bHideOnEmpty)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UPartySlotUI::NativePreConstruct()
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

void UPartySlotUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAllowEdit)
	{
		ClearButton->OnClicked.AddDynamic(this, &UPartySlotUI::ClearSlot);
	}

	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (IsValid(PartySubsystem))
	{
		StorageManager = PartySubsystem->GetStorageManager();
		if (IsValid(StorageManager))
		{
			StorageManager->OnStorageUpdated.AddUObject(this, &UPartySlotUI::RefreshDetail);
			RefreshDetail();
		}
	}
}

void UPartySlotUI::NativeDestruct()
{
	if (bAllowEdit)
	{
		ClearButton->OnClicked.Clear();
	}

	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}

bool UPartySlotUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!bAllowEdit)
	{
		return false;
	}

	UAssetDragOperation* DragOperation = Cast<UAssetDragOperation>(InOperation);
	if (!IsValid(DragOperation))
	{
		return false;
	}

	const FPrimaryAssetId& AssetId = DragOperation->AssetId;
	if (!AssetId.IsValid())
	{
		return false;
	}

	UPartySubsystem* PartySubsystem = UPartySubsystem::Get(GetWorld());
	if (!IsValid(PartySubsystem))
	{
		return false;
	}

	return PartySubsystem->TrySetPartyCharacter(CharacterSlot, AssetId);
}


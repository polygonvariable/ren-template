// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Core/Type/Runtime/InventoryInstance.h"
#include "System/InventoryStorageManager.h"
#include "System/InventorySubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/InventoryEntry.h"



void UInventoryDetailUI::InitializeDetail()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	StorageManager = InventorySubsystem->GetStorageManager(PrimarySourceId);
	if (IsValid(StorageManager) && bAutoRefresh)
	{
		StorageManager->OnStorageUpdated.AddUObject(this, &UInventoryDetailUI::RefreshDetail);
	}
}

void UInventoryDetailUI::RefreshDetail()
{
	if (!IsValid(StorageManager))
	{
		return;
	}

	const FInventoryInstance* Item = StorageManager->GetInstanceById(GetActiveAssetId(), ActiveItemId);
	if (!Item)
	{
		return;
	}

	int Quantity = Item->Quantity;
	SetCustomDetails(Item, Quantity);
}

void UInventoryDetailUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		SwitchDetail(false);
		return;
	}

	SwitchDetail(true);

	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UInventoryDetailUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UInventoryEntry* InventoryEntry = Cast<UInventoryEntry>(Entry);
	if (!IsValid(InventoryEntry))
	{
		return;
	}

	const FInventoryInstance* Item = InventoryEntry->Item;
	int Quantity = InventoryEntry->Quantity;
	SetCustomDetails(Item, Quantity);
}

void UInventoryDetailUI::SetCustomDetails(const FInventoryInstance* Item, int Quantity)
{
	if (!Item)
	{
		return;
	}

	ActiveItemId = Item->ItemId;

	ItemQuantity->SetText(FText::AsNumber(Quantity));
	AscensionDetail->InitializeDetail(Item->Ascension);
}

void UInventoryDetailUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}


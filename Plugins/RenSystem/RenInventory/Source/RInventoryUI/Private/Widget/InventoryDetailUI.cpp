// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Definition/Runtime/InventoryInstance.h"
#include "Storage/InventoryStorage.h"
#include "Subsystem/InventorySubsystem.h"
#include "Widget/AscensionDetailUI.h"
#include "Widget/InventoryEntry.h"



void UInventoryDetailUI::InitializeDetail()
{
	UInventorySubsystem* InventorySubsystem = UInventorySubsystem::Get(GetGameInstance());
	if (!IsValid(InventorySubsystem))
	{
		return;
	}

	UInventoryStorage* InventoryStorage = InventorySubsystem->GetInventory(PrimarySourceId);
	if (IsValid(InventoryStorage) && bAutoRefresh)
	{
		InventoryStorage->OnStorageUpdated.AddUObject(this, &UInventoryDetailUI::RefreshDetail);
	}

	Inventory = TWeakObjectPtr<UInventoryStorage>(InventoryStorage);
}

void UInventoryDetailUI::RefreshDetail()
{
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (!IsValid(InventoryStorage))
	{
		return;
	}

	const FInventoryInstance* Item = InventoryStorage->GetInstanceById(GetActiveAssetId(), ActiveItemId);
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
	UInventoryStorage* InventoryStorage = Inventory.Get();
	if (IsValid(InventoryStorage))
	{
		InventoryStorage->OnStorageUpdated.RemoveAll(this);
	}
	Inventory.Reset();

	Super::NativeDestruct();
}


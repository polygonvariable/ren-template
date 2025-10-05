// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryViewUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryEntry.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"
#include "RenInventory/Public/Widget/InventorySwitchUI.h"
#include "RenInventory/Public/Widget/InventoryUI.h"



void UInventoryViewUI::DisplayEnhanceItem()
{
	if (!ItemList)
	{
		LOG_ERROR(LogInventory, TEXT("InventoryCollection is invalid"));
		return;
	}

	UInventoryEntry* Entry = ItemList->GetSelectedItem();
	if (!IsValid(Entry))
	{
		LOG_ERROR(LogInventory, TEXT("Entry is invalid"));
		return;
	}

	if (IsValid(EnhanceItemUI))
	{
		EnhanceItemUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UInventoryUI* Widget = CreateWidget<UInventoryUI>(this, EnhanceItemClass);
		if (!IsValid(Widget))
		{
			LOG_ERROR(LogInventory, TEXT("EnhanceItemUI is invalid"));
			return;
		}

		Widget->AddToViewport();
		Widget->InitializeDetails(
			Entry->AssetId,
			Entry->Quantity,
			Entry->Record
		);

		EnhanceItemUI = Widget;
	}
}

TArray<UInventorySwitchUI*> UInventoryViewUI::GetDetailSwitches_Implementation() const
{
	return {};
}

void UInventoryViewUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (ItemDetail) ItemDetail->InitializeDetails(AssetId, Quantity, Record);

	const TArray<UInventorySwitchUI*>& Switches = GetDetailSwitches();
	for (UInventorySwitchUI* Switch : Switches)
	{
		if (Switch) Switch->InitializeDetails(AssetId, Quantity);
	}
}


void UInventoryViewUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ItemDetail) ItemDetail->ContainerId = ContainerId;
	if (ItemList) ItemList->QueryRule.ContainerId = ContainerId;
}

void UInventoryViewUI::NativeConstruct()
{
	if (ItemList)
	{
		ItemList->OnItemSelected.RemoveAll(this);
		ItemList->OnItemSelected.AddUObject(this, &UInventoryViewUI::InitializeDetails);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveAll(this);
		CloseButton->OnClicked.AddDynamic(this, &UInventoryViewUI::CloseWidget);
	}

	UUserWidget::NativeConstruct();
}

void UInventoryViewUI::NativeDestruct()
{
	if (ItemList) ItemList->OnItemSelected.RemoveAll(this);
	if (CloseButton) CloseButton->OnClicked.RemoveAll(this);

	UUserWidget::NativeDestruct();
}


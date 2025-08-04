// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryDetailWidget::InitializeDetail(const FName& Guid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		ResetDetail();
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	ItemGuid = Guid;

	if (AssetTitle) AssetTitle->SetText(Asset->ItemName);
	if (AssetDescription) AssetDescription->SetText(Asset->ItemDescription);
	if (AssetImage && Asset->ItemIcon.IsValid()) AssetImage->SetBrushFromSoftTexture(Asset->ItemIcon);

	if (AssetTypeWidget)
	{
		AssetTypeWidget->SetVisibility(AssetTypeVisibility.Contains(Asset->ItemType) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (Record)
	{
		if (RecordRank) RecordRank->SetText(FText::FromString(FString::FromInt(Record->EnhanceRecord.Rank)));
		if (RecordLevel) RecordLevel->SetText(FText::FromString(FString::FromInt(Record->EnhanceRecord.Level)));
		if (RecordExperience) RecordExperience->SetText(FText::FromString(FString::FromInt(Record->EnhanceRecord.Experience)));
	}

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(IsValid(Asset) ? 1 : 0);
}

void UInventoryDetailWidget::RefreshDetail()
{
	if (!ItemGuid.IsValid() || !IsValid(InventorySubsystem)) {
		ResetDetail();
		LOG_ERROR(LogTemp, "ItemGuid or InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = InventorySubsystem->GetItemRecord(ContainerId, ItemGuid);
	if (!Record)
	{
		ResetDetail();
		LOG_ERROR(LogTemp, "Record is invalid");
		return;
	}

	UInventoryAsset* Asset = InventorySubsystem->GetItemAsset(Record->ItemId);
	if (!Asset)
	{
		ResetDetail();
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	HandleDetail(Record, Asset);
}

void UInventoryDetailWidget::ResetDetail()
{
	ItemGuid = NAME_None;

	if (DetailSwitcher)
	{
		DetailSwitcher->SetActiveWidgetIndex(0);
	}
}

void UInventoryDetailWidget::HandleDetail(const FInventoryRecord* Record, UInventoryAsset* Asset)
{

}



void UInventoryDetailWidget::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* InventorySubsystemPtr = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(InventorySubsystemPtr))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		InventorySubsystem = InventorySubsystemPtr;
	}

	Super::NativeConstruct();
}

void UInventoryDetailWidget::NativeDestruct()
{
	InventorySubsystem = nullptr;

	Super::NativeDestruct();
}


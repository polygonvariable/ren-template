// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

// Project Headers
#include "RenAsset/Public/Inventory/InventoryAsset.h"

#include "RenCore/Public/Macro/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryDetailWidget::InitializeDetail(const FName& Guid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	ItemGuid = Guid;

	HandleDetail(Record, Asset);

	if (ItemId) ItemId->SetText(FText::FromName(Guid));
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

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(0);

	SetPrimaryDetail(FText::GetEmpty(), FText::GetEmpty(), nullptr);
	SetSecondaryDetail();
}

void UInventoryDetailWidget::HandleDetail(const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		ResetDetail();
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	SetPrimaryDetail(
		Asset->ItemName,
		Asset->ItemDescription,
		Asset->ItemIcon
	);

	if (Record)
	{
		SetSecondaryDetail(
			Record->ItemQuantity,
			Record->EnhanceRecord.Rank,
			Record->EnhanceRecord.Level,
			Record->EnhanceRecord.Experience
		);
	}
	else
	{
		SetSecondaryDetail();
	}

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(1);
}

void UInventoryDetailWidget::SetPrimaryDetail(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image)
{
	if (ItemName) ItemName->SetText(Title);
	if (ItemDescription) ItemDescription->SetText(Description);
	if (ItemIcon) ItemIcon->SetBrushFromSoftTexture(Image);
}

void UInventoryDetailWidget::SetSecondaryDetail(int Quantity, int Rank, int Level, int Experience)
{
	if (ItemQuantity) ItemQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	if (ItemRank) ItemRank->SetText(FText::FromString(FString::FromInt(Rank)));
	if (ItemLevel) ItemLevel->SetText(FText::FromString(FString::FromInt(Level)));
	if (ItemExperience) ItemExperience->SetText(FText::FromString(FString::FromInt(Experience)));
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


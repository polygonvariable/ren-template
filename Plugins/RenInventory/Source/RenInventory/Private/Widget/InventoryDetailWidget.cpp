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



void UInventoryDetailWidget::InitializeDetail(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	ActiveItemGuid = ItemGuid;

	HandleDetail(Record, Asset);
}

void UInventoryDetailWidget::RefreshDetail()
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!ActiveItemGuid.IsValid() || !IsValid(Subsystem)) {
		ResetDetail();
		LOG_ERROR(LogTemp, "ItemGuid or InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = Subsystem->GetItemRecord(ContainerId, ActiveItemGuid);
	if (!Record)
	{
		ResetDetail();
		LOG_ERROR(LogTemp, "Record is invalid");
		return;
	}

	UInventoryAsset* Asset = Subsystem->GetItemAsset(Record->ItemId);
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
	ActiveItemGuid = NAME_None;

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
	if (ItemGuidText) ItemGuidText->SetText(FText::FromName(ActiveItemGuid));
	if (ItemNameText) ItemNameText->SetText(Title);
	if (ItemDescriptionText) ItemDescriptionText->SetText(Description);
	if (ItemImage) ItemImage->SetBrushFromSoftTexture(Image);
}

void UInventoryDetailWidget::SetSecondaryDetail(int Quantity, int Rank, int Level, int Experience)
{
	if (ItemQuantityText) ItemQuantityText->SetText(FText::FromString(FString::FromInt(Quantity)));
	if (ItemRankText) ItemRankText->SetText(FText::FromString(FString::FromInt(Rank)));
	if (ItemLevelText) ItemLevelText->SetText(FText::FromString(FString::FromInt(Level)));
	if (ItemExperienceText) ItemExperienceText->SetText(FText::FromString(FString::FromInt(Experience)));
}

void UInventoryDetailWidget::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Subsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Subsystem))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Subsystem->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetail(); });
			Subsystem->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, FInventoryRecord Record)			{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetail(); });
			Subsystem->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetail(); });
		}

		InventorySubsystem = Subsystem;
	}

	Super::NativeConstruct();
}

void UInventoryDetailWidget::NativeDestruct()
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (IsValid(Subsystem)) {
		Subsystem->OnItemAdded.RemoveAll(this);
		Subsystem->OnItemRemoved.RemoveAll(this);
		Subsystem->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}


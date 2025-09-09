// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailWidget.h"

// Engine Headers
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryDetailWidget::InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset)
{
	if (!Asset)
	{
		LOG_ERROR(LogTemp, "Asset is invalid");
		return;
	}

	ActiveAsset = Asset;
	ActiveItemGuid = ItemGuid;

	HandleDetails(Record);
}

void UInventoryDetailWidget::RefreshDetails()
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem)) {
		ResetDetails();
		LOG_ERROR(LogTemp, "ItemGuid or InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = Subsystem->GetItemRecord(ContainerId, ActiveItemGuid);
	if (!Record)
	{
		ResetDetails();
		LOG_ERROR(LogTemp, "Record is invalid");
		return;
	}

	HandleDetails(Record);
}

void UInventoryDetailWidget::ResetDetails()
{
	ActiveItemGuid = NAME_None;
	ActiveAsset = nullptr;

	FText Empty = FText::GetEmpty();

	SetPrimaryDetails(Empty, Empty, nullptr);
	SetSecondaryDetails(Empty, 0, 0, 0, 0);

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(0);
}

void UInventoryDetailWidget::HandleDetails(const FInventoryRecord* Record)
{
	if (!ActiveAsset)
	{
		ResetDetails();
		LOG_ERROR(LogTemp, "ActiveAsset is invalid");
		return;
	}

	SetPrimaryDetails(
		ActiveAsset->ItemName,
		ActiveAsset->ItemDescription,
		ActiveAsset->ItemIcon
	);

	if (Record)
	{
		SetSecondaryDetails(
			FText::FromName(ActiveItemGuid),
			Record->ItemQuantity,
			Record->EnhanceRecord.Rank,
			Record->EnhanceRecord.Level,
			Record->EnhanceRecord.Experience
		);
	}
	else
	{
		SetSecondaryDetails(FText::GetEmpty(), 0, 0, 0, 0);
	}

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(1);
}

void UInventoryDetailWidget::SetPrimaryDetails(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image)
{
	if (ItemNameText) ItemNameText->SetText(Title);
	if (ItemDescriptionText) ItemDescriptionText->SetText(Description);
	if (ItemImage) ItemImage->SetBrushFromSoftTexture(Image);
}

void UInventoryDetailWidget::SetSecondaryDetails(const FText& Guid, int Quantity)
{
	if (ItemGuidText) ItemGuidText->SetText(Guid);
	if (ItemQuantityText) ItemQuantityText->SetText(FText::FromString(FString::FromInt(Quantity)));
}

void UInventoryDetailWidget::SetSecondaryDetails(const FText& Guid, int Quantity, int Rank, int Level, int Experience)
{
	SetSecondaryDetails(Guid, Quantity);
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
			Subsystem->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Subsystem->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, FInventoryRecord Record)			{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Subsystem->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, FName ItemGuid, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = Subsystem;
	}

	Super::NativeConstruct();
}

void UInventoryDetailWidget::NativeDestruct()
{
	ActiveAsset = nullptr;

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (IsValid(Subsystem)) {
		Subsystem->OnItemAdded.RemoveAll(this);
		Subsystem->OnItemRemoved.RemoveAll(this);
		Subsystem->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}


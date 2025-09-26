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

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryDetailWidget::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	ActiveAssetId = AssetId;
	ActiveRecordId = Record ? Record->ItemId : NAME_None;

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

	const FInventoryRecord* Record = Subsystem->GetRecordById(ContainerId, ActiveAssetId, ActiveRecordId);
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
	ActiveAssetId = FPrimaryAssetId();
	ActiveRecordId = NAME_None;

	FText EmptyText = FText::GetEmpty();

	SetPrimaryDetails(EmptyText, EmptyText, nullptr);
	SetSecondaryDetails(0, 0, 0, 0);

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(0);
}

void UInventoryDetailWidget::HandleDetails(const FInventoryRecord* Record)
{
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, ActiveAssetId, [this](FPrimaryAssetId AssetId, UObject* LoadedAsset)
		{
			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (IsValid(InventoryAsset))
			{
				SetPrimaryDetails(
					InventoryAsset->ItemName,
					InventoryAsset->ItemDescription,
					InventoryAsset->ItemIcon
				);
			}
		}
	);

	if (Record)
	{
		SetSecondaryDetails(
			Record->ItemQuantity,
			Record->EnhanceRecord.Rank,
			Record->EnhanceRecord.Level,
			Record->EnhanceRecord.Experience
		);
	}
	else
	{
		SetSecondaryDetails(0, 0, 0, 0);
	}

	if (DetailSwitcher) DetailSwitcher->SetActiveWidgetIndex(1);
}

void UInventoryDetailWidget::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
	if (ItemNameText) ItemNameText->SetText(Title);
	if (ItemDescriptionText) ItemDescriptionText->SetText(Description);
	if (ItemImage) ItemImage->SetBrushFromSoftTexture(Image);
}

void UInventoryDetailWidget::SetSecondaryDetails(int Quantity)
{
	if (ItemQuantityText) ItemQuantityText->SetText(FText::FromString(FString::FromInt(Quantity)));
}

void UInventoryDetailWidget::SetSecondaryDetails(int Quantity, int Rank, int Level, int Experience)
{
	SetSecondaryDetails(Quantity);
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
			Subsystem->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Subsystem->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Subsystem->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
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


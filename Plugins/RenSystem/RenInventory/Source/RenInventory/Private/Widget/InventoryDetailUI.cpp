// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryDetailUI.h"

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
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



void UInventoryDetailUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	ActiveAssetId = AssetId;
	ActiveRecordId = (Record) ? Record->ItemId : NAME_None;

	FetchDetails(Record, Quantity);
}

void UInventoryDetailUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory)) {
		SwitchDetails(false);
		return;
	}

	const FInventoryRecord* Record = Inventory->GetRecordById(ContainerId, ActiveAssetId, ActiveRecordId);
	int Quantity = Inventory->GetTotalQuantity(ContainerId, ActiveAssetId);
	if (!Record)
	{
		SwitchDetails(false);
		return;
	}

	FetchDetails(Record, Quantity);
}

void UInventoryDetailUI::ResetDetails()
{
	ActiveAssetId = FPrimaryAssetId();
	ActiveRecordId = NAME_None;
	
	SwitchDetails(false);
}

void UInventoryDetailUI::FetchDetails(const FInventoryRecord* Record, int Quantity)
{
	FEnhanceRecord Enhance = (Record) ? Record->Enhancement : FEnhanceRecord();

	TWeakObjectPtr<UInventoryDetailUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, Enhance, Quantity](bool, UObject* LoadedAsset)
		{
			UInventoryDetailUI* Owner = WeakThis.Get();
			if (!IsValid(Owner))
			{
				return;
			}

			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (!IsValid(InventoryAsset))
			{
				Owner->SwitchDetails(false);
			}

			Owner->SetPrimaryDetails(
				InventoryAsset->DisplayName,
				InventoryAsset->Description,
				InventoryAsset->Icon
			);
			Owner->SetSecondaryDetails(Quantity, Enhance);
			Owner->SwitchDetails(true);
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, ActiveAssetId, MoveTemp(AsyncCallback));
}

void UInventoryDetailUI::SwitchDetails(bool bPrimary)
{
	if (DetailSwitch)
	{
		DetailSwitch->SetActiveWidgetIndex((bPrimary) ? 1 : 0);
	}
}

void UInventoryDetailUI::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
	if (ItemName) ItemName->SetText(Title);
	if (ItemDescription) ItemDescription->SetText(Description);
	if (ItemIcon) ItemIcon->SetBrushFromSoftTexture(Image);
}

void UInventoryDetailUI::SetSecondaryDetails(int Quantity, const FEnhanceRecord& Enhance)
{
	if (ItemQuantity) ItemQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	if (ItemRank) ItemRank->SetText(FText::FromString(FString::FromInt(Enhance.Rank)));
	if (ItemLevel) ItemLevel->SetText(FText::FromString(FString::FromInt(Enhance.Level)));
	if (ItemExperience) ItemExperience->SetText(FText::FromString(FString::FromInt(Enhance.Experience)));
}

void UInventoryDetailUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Inventory))
		{
			LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Inventory->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)		{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = Inventory;
	}

	Super::NativeConstruct();
}

void UInventoryDetailUI::NativeDestruct()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}



void UInventoryGlossaryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity)
{
	ActiveAssetId = AssetId;
	FetchDetails(Quantity);
}

void UInventoryGlossaryUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory)) {
		SwitchDetails(false);
		return;
	}

	int Quantity = Inventory->GetTotalQuantity(ContainerId, ActiveAssetId);
	FetchDetails(Quantity);
}

void UInventoryGlossaryUI::ResetDetails()
{
	SwitchDetails(false);
}

void UInventoryGlossaryUI::FetchDetails(int Quantity)
{
	TWeakObjectPtr<UInventoryGlossaryUI> WeakThis(this);

	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, Quantity](bool, UObject* LoadedAsset)
		{
			UInventoryGlossaryUI* Owner = WeakThis.Get();
			if (!IsValid(Owner))
			{
				return;
			}

			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (!IsValid(InventoryAsset))
			{
				Owner->SwitchDetails(false);
				return;
			}

			Owner->SetPrimaryDetails(
				InventoryAsset->DisplayName,
				InventoryAsset->Description,
				InventoryAsset->Icon
			);
			Owner->SetSecondaryDetails(Quantity);
			Owner->SwitchDetails(true);
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, ActiveAssetId, MoveTemp(AsyncCallback));
}

void UInventoryGlossaryUI::SwitchDetails(bool bPrimary)
{
	if (DetailSwitch)
	{
		DetailSwitch->SetActiveWidgetIndex((bPrimary) ? 1 : 0);
	}
}

void UInventoryGlossaryUI::SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image)
{
	if (ItemName) ItemName->SetText(Title);
	if (ItemDescription) ItemDescription->SetText(Description);
	if (ItemIcon) ItemIcon->SetBrushFromSoftTexture(Image);
}

void UInventoryGlossaryUI::SetSecondaryDetails(int Quantity)
{
	if (ItemQuantity) ItemQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
}

void UInventoryGlossaryUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Inventory))
		{
			LOG_ERROR(LogInventory, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Inventory->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = Inventory;
	}

	Super::NativeConstruct();
}

void UInventoryGlossaryUI::NativeDestruct()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory)) {
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	Super::NativeDestruct();
}


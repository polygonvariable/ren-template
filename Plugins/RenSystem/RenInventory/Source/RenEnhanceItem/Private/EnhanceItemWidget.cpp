// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemWidget.h"

// Engine Headers
#include "Components/WidgetSwitcher.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceLibrary.h"

#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"
#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionWidget.h"
#include "RenInventory/Public/Widget/InventoryDetailWidget.h"
#include "RenInventory/Public/Widget/InventoryEntryObject.h"

#include "RenEnhanceItem/Public/EnhanceItemSubsystem.h"



void UEnhanceItemWidget::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!Record || !AssetId.IsValid())
	{
		return;
	}

	ActiveAssetId = AssetId;
	ActiveItemId = Record->ItemId;
	ActiveEnhanceRecord = Record->EnhanceRecord;

	TWeakObjectPtr<UEnhanceItemWidget> WeakThis(this);
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, [WeakThis](FPrimaryAssetId AssetId, UObject* LoadedAsset)
		{
			if (!WeakThis.IsValid()) return;
			WeakThis.Get()->HandleAssetLoaded(AssetId, LoadedAsset);
		}
	);

	if (InventoryDetail)
	{
		InventoryDetail->InitializeDetails(AssetId, Quantity, Record);
	}
}

void UEnhanceItemWidget::ResetDetails()
{
	ActiveItemId = NAME_None;
	ActiveAsset = nullptr;
}

void UEnhanceItemWidget::RefreshDetails()
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem)) {
		LOG_ERROR(LogTemp, "InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = Subsystem->GetRecordById(ContainerId, ActiveAssetId, ActiveItemId);
	if (!Record)
	{
		LOG_ERROR(LogTemp, "Record is invalid");
		return;
	}

	ActiveEnhanceRecord = Record->EnhanceRecord;

	HandleRankUpDisplay();
}


void UEnhanceItemWidget::TryLevelUp(const FPrimaryAssetId& AssetId, FName ItemId)
{
	UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		Subsystem->LevelUpItem(ContainerId, ActiveAssetId, ActiveItemId, AssetId, ItemId);
	}
}

void UEnhanceItemWidget::TryRankUp()
{
	UEnhanceItemSubsystem* Subsystem = EnhanceItemSubsystem.Get();
	if (IsValid(Subsystem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		Subsystem->RankUpItem(ContainerId, ActiveAssetId, ActiveItemId);
	}
}


void UEnhanceItemWidget::HandleLevelUpDisplay()
{
	if (LevelUpCollection && ActiveAsset && ActiveItemId.IsValid())
	{
		const FExchangeRule& EnhanceRules = ActiveAsset->EnhanceRules;
		const TMap<FPrimaryAssetId, int>& RequiredAssets = EnhanceRules.RequiredAssets;
		const TMap<FName, int>& RequiredArbitrary = EnhanceRules.RequiredArbitrary;

		TArray<FPrimaryAssetId> AssetIds;
		RequiredAssets.GetKeys(AssetIds);

		TArray<FName> AssetTypes;
		RequiredArbitrary.GetKeys(AssetTypes);

		UFilterGroup* FilterRule = LevelUpCollection->FilterRule;
		if (FilterRule)
		{
			UFilterAssetCriterion* AssetCriterion = FilterRule->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
			UFilterTextCriterion* AssetTypeCriterion = FilterRule->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::AssetType);
			UFilterTextCriterion* ItemIdCriterion = FilterRule->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::ItemId);

			if (AssetCriterion)
			{
				AssetCriterion->Included.Append(AssetIds);
			}
			if (AssetTypeCriterion)
			{
				AssetTypeCriterion->Included.Append(AssetTypes);
			}
			if (ItemIdCriterion)
			{
				ItemIdCriterion->Included.Add(ActiveItemId);
			}
		}

		LevelUpCollection->DisplayItems();
	}
}

void UEnhanceItemWidget::HandleRankUpDisplay()
{
	if (!ActiveAsset)
	{
		return;
	}

	int CurrentRank = ActiveEnhanceRecord.Rank;

	bool bCanRankUp = UEnhanceLibrary::CanRankUp(
		ActiveEnhanceRecord,
		ActiveAsset->GetLevelInterval(CurrentRank),
		ActiveAsset->MaxLevel
	);

	if (EnhanceSwitcher)
	{
		EnhanceSwitcher->SetActiveWidgetIndex(bCanRankUp ? 1 : 0);
	}

	if (RankUpCollection)
	{
		RankUpCollection->ClearItems();

		const TArray<FExchangeRule>& RankingRules = ActiveAsset->RankingRules;
		if (bCanRankUp && RankingRules.IsValidIndex(CurrentRank))
		{
			TMap<FPrimaryAssetId, FInstancedStruct> Payloads;

			const TMap<FPrimaryAssetId, int>& RequiredAssets = RankingRules[CurrentRank].RequiredAssets;

			for (auto& Pair : RequiredAssets)
			{
				const FPrimaryAssetId& AssetId = Pair.Key;
				int Quantity = Pair.Value;

				FInventoryPayloadQuantity Payload;
				Payload.Quantity = Quantity;

				Payloads.Add(AssetId, FInstancedStruct::Make(Payload));
			}

			TArray<FPrimaryAssetId> AssetIds;
			RequiredAssets.GetKeys(AssetIds);

			RankUpCollection->SetPayloads(Payloads);
			//RankUpCollection->FilterRule.FilterAsset.Included.Append(AssetIds);
			RankUpCollection->DisplayItems();
		}
	}
}



void UEnhanceItemWidget::HandleAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset)
{
	UEnhanceableAsset* ItemAsset = Cast<UEnhanceableAsset>(LoadedAsset);
	if (ItemAsset)
	{
		ActiveAsset = ItemAsset;

		HandleLevelUpDisplay();
	}
}

void UEnhanceItemWidget::HandleItemSelected(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (Record)
	{
		TryLevelUp(AssetId, Record->ItemId);
	}
}

void UEnhanceItemWidget::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UEnhanceItemSubsystem* EnhanceItemSubsystemPtr = GetGameInstance()->GetSubsystem<UEnhanceItemSubsystem>();
		if (IsValid(EnhanceItemSubsystemPtr))
		{
			EnhanceItemSubsystem = EnhanceItemSubsystemPtr;
		}

		UInventorySubsystem* InventorySubsystemPtr = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(InventorySubsystemPtr))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			InventorySubsystemPtr->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			InventorySubsystemPtr->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			InventorySubsystemPtr->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = InventorySubsystemPtr;
	}

	if (LevelUpCollection)
	{
		LevelUpCollection->OnItemSelected.AddUObject(this, &UEnhanceItemWidget::HandleItemSelected);
	}

	Super::NativeConstruct();
}

void UEnhanceItemWidget::NativeDestruct()
{
	ResetDetails();

	if (LevelUpCollection)
	{
		LevelUpCollection->OnItemSelected.RemoveAll(this);
	}

	UInventorySubsystem* InventorySubsystemPtr = InventorySubsystem.Get();
	if (IsValid(InventorySubsystemPtr))
	{
		InventorySubsystemPtr->OnItemAdded.RemoveAll(this);
		InventorySubsystemPtr->OnItemRemoved.RemoveAll(this);
		InventorySubsystemPtr->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	EnhanceItemSubsystem.Reset();

	Super::NativeDestruct();
}


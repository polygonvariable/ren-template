// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemWidget.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceLibrary.h"

#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/Asset/Category/EnhanceableAsset.h"
#include "RenInventory/Public/Asset/Type/EnhanceAsset.h"

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
			UEnhanceItemWidget* Outer = WeakThis.Get();
			if (IsValid(Outer))
			{
				Outer->HandleAssetLoaded(AssetId, LoadedAsset);
			}
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
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory)) {
		LOG_ERROR(LogTemp, "InventorySubsystem is invalid");
		return;
	}

	const FInventoryRecord* Record = Inventory->GetRecordById(ContainerId, ActiveAssetId, ActiveItemId);
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
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	if (IsValid(EnhanceItem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		EnhanceItem->LevelUpItem(ContainerId, ActiveAssetId, ActiveItemId, AssetId, ItemId);
	}
}

void UEnhanceItemWidget::TryRankUp()
{
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	if (IsValid(EnhanceItem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		EnhanceItem->RankUpItem(ContainerId, ActiveAssetId, ActiveItemId);
	}
}


void UEnhanceItemWidget::HandleLevelUpDisplay()
{
	IEnhanceProviderInterface* EnhanceProvider = ActiveAsset.Get();
	if (!IsValid(LevelUpCollection) || !EnhanceProvider || !ActiveItemId.IsValid())
	{
		return;
	}

	const FExchangeRule& EnhanceRules = EnhanceProvider->GetEnhanceRules();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = EnhanceRules.RequiredAssets;
	const TMap<FName, int>& RequiredArbitrary = EnhanceRules.RequiredArbitrary;

	TArray<FPrimaryAssetId> AssetIds;
	RequiredAssets.GetKeys(AssetIds);

	TArray<FName> AssetTypes;
	RequiredArbitrary.GetKeys(AssetTypes);

	UFilterGroup* FilterRule = LevelUpCollection->FilterRule;
	if (IsValid(FilterRule))
	{
		UFilterAssetCriterion* AssetCriterion = FilterRule->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
		UFilterTextCriterion* AssetTypeCriterion = FilterRule->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::AssetType);
		UFilterTextCriterion* ItemIdCriterion = FilterRule->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::ItemId);

		if (IsValid(AssetCriterion))
		{
			AssetCriterion->Included.Append(AssetIds);
		}
		if (IsValid(AssetTypeCriterion))
		{
			AssetTypeCriterion->Included.Append(AssetTypes);
		}
		if (IsValid(ItemIdCriterion))
		{
			ItemIdCriterion->Included.Add(ActiveItemId);
		}
	}

	LevelUpCollection->DisplayItems();
}

void UEnhanceItemWidget::HandleRankUpDisplay()
{
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	IEnhanceProviderInterface* EnhanceProvider = ActiveAsset.Get();

	if (!IsValid(EnhanceItem) || !EnhanceProvider)
	{
		return;
	}

	bool bCanRankUp = EnhanceItem->CanRankUp(ActiveEnhanceRecord, EnhanceProvider);

	if (EnhanceSwitcher)
	{
		EnhanceSwitcher->SetActiveWidgetIndex(bCanRankUp ? 1 : 0);
	}

	if (RankUpCollection)
	{
		RankUpCollection->ClearItems();

		int CurrentRank = ActiveEnhanceRecord.Rank;

		const TArray<FExchangeRule>& RankingRules = ActiveAsset->GetRankingRules();
		bool bIsRankValid = bCanRankUp && RankingRules.IsValidIndex(CurrentRank);

		if (bIsRankValid)
		{
			TMap<FPrimaryAssetId, FInstancedStruct> Payloads;

			const TMap<FPrimaryAssetId, int>& RequiredAssets = RankingRules[CurrentRank].RequiredAssets;
			for (const TPair<FPrimaryAssetId, int>& Kv : RequiredAssets)
			{
				const FPrimaryAssetId& AssetId = Kv.Key;
				int Quantity = Kv.Value;

				FInventoryPayloadQuantity Payload;
				Payload.Quantity = Quantity;

				Payloads.Add(AssetId, FInstancedStruct::Make(Payload));
			}

			TArray<FPrimaryAssetId> AssetIds;
			RequiredAssets.GetKeys(AssetIds);

			UFilterGroup* FilterRule = RankUpCollection->FilterRule;
			if (IsValid(FilterRule))
			{
				UFilterAssetCriterion* AssetCriterion = FilterRule->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
				if (IsValid(AssetCriterion))
				{
					AssetCriterion->Included.Empty();
					AssetCriterion->Included.Append(AssetIds);
				}
			}

			RankUpCollection->SetPayloads(Payloads);
			RankUpCollection->DisplayItems();
		}
	}
}



void UEnhanceItemWidget::HandleAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset)
{
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(LoadedAsset);
	if (!EnhanceProvider)
	{
		return;
	}

	ActiveAsset = TWeakInterfacePtr<IEnhanceProviderInterface>(EnhanceProvider);

	HandleLevelUpDisplay();
	HandleRankUpDisplay();
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
		UEnhanceItemSubsystem* EnhanceItem = GetGameInstance()->GetSubsystem<UEnhanceItemSubsystem>();
		if (IsValid(EnhanceItem))
		{
			EnhanceItemSubsystem = EnhanceItem;
		}

		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (!IsValid(Inventory))
		{
			LOG_ERROR(LogTemp, TEXT("InventorySubsystem is invalid"));
			return;
		}

		if (bAutoRefresh)
		{
			Inventory->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			Inventory->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId& AssetId, const FInventoryRecord* Record)	{ if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
		}

		InventorySubsystem = Inventory;
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

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	EnhanceItemSubsystem.Reset();

	Super::NativeDestruct();
}


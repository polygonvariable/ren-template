// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "CraftItemUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreCraft/Public/CraftProviderInterface.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"
#include "RenInventory/Public/InventoryEntry.h"

#include "RenCraftItem/Public/CraftItemSubsystem.h"

#include "RCoreCounter/Public/CounterContainer.h"
#include "RenCounter/Public/CounterSubsystem.h"



int UCraftItemUI::GetCraftingQuantity() const
{
	return 1;
}


void UCraftItemUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!IsValid(AssetManager) || !InventoryDetail || !CraftButton || !RequiredCollection)
	{
		return;
	}

	LockControls(true);

	ActiveAssetId = AssetId;

	InventoryDetail->InitializeDetails(AssetId, Quantity);

	TWeakObjectPtr<UCraftItemUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis](bool, UObject* LoadedObject)
		{
			UCraftItemUI* Outer = WeakThis.Get();
			if (IsValid(Outer))
			{
				Outer->DisplayRequiredItems(LoadedObject);
			}
		};
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

void UCraftItemUI::DisplayStockItems()
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!CraftCollection || !IsValid(Counter))
	{
		return;
	}

	const TMap<FPrimaryAssetId, FExchangeQuota>& Stock = CraftCatalog.Stock;

	if (bEnableCounter)
	{
		CraftCollection->ClearPayloads();

		for (const TPair<FPrimaryAssetId, FExchangeQuota>& Kv : Stock)
		{
			const FPrimaryAssetId& AssetId = Kv.Key;

			FExchangeRecord NewRecord;
			NewRecord.Quota = Kv.Value;

			const FCounterRecord* Record = Counter->GetCounterRecord(CraftId, AssetId.PrimaryAssetName);
			if (Record)
			{
				NewRecord.Quantity = Record->Value;
				NewRecord.LastExchangeTime = Record->Timestamp;
			}
			else
			{
				NewRecord.Quantity = 0;
				NewRecord.LastExchangeTime = FDateTime::MinValue();
			}

			CraftCollection->AddPayload(AssetId, FInstancedStruct::Make(NewRecord));
		}
	}

	TArray<FPrimaryAssetId> AssetIds;
	Stock.GetKeys(AssetIds);

	UFilterAssetCriterion* AssetCriterion = CraftCollection->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	CraftCollection->ClearItems();
	CraftCollection->DisplayItems();
}

void UCraftItemUI::DisplayRequiredItems(UObject* LoadedAsset)
{
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(LoadedAsset);
	if (!CraftProvider || !RequiredCollection)
	{
		return;
	}

	RequiredCollection->ClearPayloads();

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule.RequiredAssets;

	for (const TPair<FPrimaryAssetId, int>& Kv : RequiredAssets)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value;

		FInventoryPayloadQuantity Payload;
		Payload.Quantity = Quantity;

		RequiredCollection->AddPayload(AssetId, FInstancedStruct::Make(Payload));
	}

	TArray<FPrimaryAssetId> AssetIds;
	RequiredAssets.GetKeys(AssetIds);

	UFilterAssetCriterion* AssetCriterion = RequiredCollection->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	RequiredCollection->ClearItems();
	RequiredCollection->DisplayItems();

	LockControls(false);

	// UInventorySubsystem* Inventory = InventorySubsystem.Get();
	// int MaxMultiplier = Inventory->GetMaxMultiplier(ContainerId, RequiredAssets);
}


void UCraftItemUI::CraftItem()
{
	UCraftItemSubsystem* CraftItem = CraftItemSubsystem.Get();
	if (!IsValid(CraftItem))
	{
		return;
	}

	int Quantity = GetCraftingQuantity();
	if (!bEnableCounter)
	{
		CraftItem->CraftItem(ContainerId, ActiveAssetId, Quantity);
		return;
	}

	FExchangeQuota* Quota = CraftCatalog.Stock.Find(ActiveAssetId);
	if (!Quota)
	{
		return;
	}

	CraftItem->CraftItem(CraftId, ContainerId, ActiveAssetId, Quantity, *Quota);
}

void UCraftItemUI::HandleItemCrafted(bool bSuccess)
{
	if (bSuccess)
	{
		DisplayStockItems();

		if (InventoryDetail) InventoryDetail->RefreshDetails();
		if (RequiredCollection) RequiredCollection->RefreshItems();
	}
}


void UCraftItemUI::LockControls(bool bLock)
{
	if (CraftButton) CraftButton->SetIsEnabled(!bLock);
	if (CraftCollection) CraftCollection->SetIsEnabled(!bLock);
	if (RequiredCollection) RequiredCollection->SetIsEnabled(!bLock);
}

void UCraftItemUI::LockControls(TArray<UWidget*> Widgets, bool bLock)
{
	for (UWidget* Widget : Widgets)
	{
		Widget->SetIsEnabled(!bLock);
	}
}


void UCraftItemUI::NativePreConstruct()
{
	if (CraftCollection)
	{
		CraftCollection->bAutoRefresh = false;
		CraftCollection->bEnablePayloads = true;
		CraftCollection->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
		CraftCollection->QueryRule.ContainerId = ContainerId;
	}

	if (RequiredCollection)
	{
		RequiredCollection->bAutoRefresh = false;
		RequiredCollection->bEnablePayloads = true;
		RequiredCollection->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
		RequiredCollection->QueryRule.ContainerId = ContainerId;
	}

	if (InventoryDetail)
	{
		InventoryDetail->bAutoRefresh = false;
		InventoryDetail->ContainerId = ContainerId;
	}
}

void UCraftItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		CounterSubsystem = GameInstance->GetSubsystem<UCounterSubsystem>();
		InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();

		UCraftItemSubsystem* CraftItem = GameInstance->GetSubsystem<UCraftItemSubsystem>();
		if (IsValid(CraftItem))
		{
			CraftItem->OnItemCrafted.AddUObject(this, &UCraftItemUI::HandleItemCrafted);
			CraftItemSubsystem = CraftItem;
		}
	}

	if (CraftCollection)
	{
		CraftCollection->OnItemSelected.AddUObject(this, &UCraftItemUI::InitializeDetails);
	}

	Super::NativeConstruct();
}

void UCraftItemUI::NativeDestruct()
{
	CounterSubsystem.Reset();
	InventorySubsystem.Reset();

	UCraftItemSubsystem* CraftItem = CraftItemSubsystem.Get();
	if (IsValid(CraftItem))
	{
		CraftItem->OnItemCrafted.RemoveAll(this);
	}
	CraftItemSubsystem.Reset();

	if (CraftCollection)
	{
		CraftCollection->OnItemSelected.RemoveAll(this);
	}

	Super::NativeDestruct();
}


void UCraftItemEntryUI::SetTertiaryDetails(UInventoryEntry* Entry)
{
	if (!Entry)
	{
		return;
	}

	const FExchangeRecord* Payload = Entry->Payload.GetPtr<FExchangeRecord>();
	if (!Payload)
	{
		return;
	}

	SetSecondaryDetails(Payload->Quantity);
	if (ItemLimit) ItemLimit->SetText(FText::FromString(FString::FromInt(Payload->Quota.Limit)));
}


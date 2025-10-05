// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "CraftItemUI.h"

// Engine Headers
#include "Components/TextBlock.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"


#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreCraft/Public/CraftProviderInterface.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryEntry.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"

#include "RenCraftItem/Public/CraftItemSubsystem.h"

#include "RCoreCounter/Public/CounterContainer.h"
#include "RenCounter/Public/CounterSubsystem.h"



int UCraftItemUI::GetCraftingQuantity() const
{
	return 1;
}

void UCraftItemUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!IsValid(AssetManager) || !ItemDetail || !CraftButton || !RequiredList)
	{
		return;
	}

	LockControls(true);

	ActiveAssetId = AssetId;

	ItemDetail->InitializeDetails(AssetId, Quantity);

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
	if (!CraftingList || !IsValid(Counter))
	{
		return;
	}

	const TMap<FPrimaryAssetId, FExchangeQuota>& Stock = CraftCatalog.Stock;

	if (bEnableCounter)
	{
		CraftingList->ClearPayloads();

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

			CraftingList->AddPayload(AssetId, FInstancedStruct::Make(NewRecord));
		}
	}

	UFilterAssetCriterion* AssetCriterion = CraftingList->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		TArray<FPrimaryAssetId> AssetIds;
		Stock.GetKeys(AssetIds);

		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	CraftingList->RefreshItems();
}

void UCraftItemUI::DisplayRequiredItems(UObject* LoadedAsset)
{
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(LoadedAsset);
	if (!CraftProvider || !RequiredList)
	{
		return;
	}

	RequiredList->ClearPayloads();

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule.RequiredAssets;

	for (const TPair<FPrimaryAssetId, int>& Kv : RequiredAssets)
	{
		const FPrimaryAssetId& AssetId = Kv.Key;
		int Quantity = Kv.Value;

		FInventoryPayloadQuantity Payload;
		Payload.Quantity = Quantity;

		RequiredList->AddPayload(AssetId, FInstancedStruct::Make(Payload));
	}

	UFilterAssetCriterion* AssetCriterion = RequiredList->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		TArray<FPrimaryAssetId> AssetIds;
		RequiredAssets.GetKeys(AssetIds);

		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	RequiredList->RefreshItems();

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

		//if (ItemDetail) ItemDetail->RefreshDetails();
		//if (RequiredList) RequiredList->RefreshItems();
	}
}


void UCraftItemUI::LockControls(bool bLock)
{
	if (CraftButton) CraftButton->SetIsEnabled(!bLock);
	if (CraftingList) CraftingList->SetIsEnabled(!bLock);
	if (RequiredList) RequiredList->SetIsEnabled(!bLock);
}

void UCraftItemUI::CloseWidget()
{
	RemoveFromParent();
}


void UCraftItemUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CraftingList)
	{
		CraftingList->bAutoRefresh = false;
		CraftingList->bEnablePayloads = true;
		CraftingList->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
		CraftingList->QueryRule.ContainerId = ContainerId;
	}

	if (RequiredList)
	{
		RequiredList->bAutoRefresh = false;
		RequiredList->bEnablePayloads = true;
		RequiredList->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
		RequiredList->QueryRule.ContainerId = ContainerId;
	}

	if (ItemDetail)
	{
		ItemDetail->bAutoRefresh = false;
		ItemDetail->ContainerId = ContainerId;
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

	if (CraftingList)
	{
		CraftingList->OnItemSelected.RemoveAll(this);
		CraftingList->OnItemSelected.AddUObject(this, &UCraftItemUI::InitializeDetails);
	}

	if (CraftButton)
	{
		CraftButton->OnClicked.RemoveAll(this);
		CraftButton->OnClicked.AddDynamic(this, &UCraftItemUI::CraftItem);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveAll(this);
		CloseButton->OnClicked.AddDynamic(this, &UCraftItemUI::CloseWidget);
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

	if (CraftingList)
	{
		CraftingList->OnItemSelected.RemoveAll(this);
	}

	if (CraftButton)
	{
		CraftButton->OnClicked.RemoveAll(this);
	}

	Super::NativeDestruct();
}


void UCraftItemEntryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	TWeakObjectPtr<UCraftItemEntryUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, Quantity](bool, UObject* LoadedAsset)
		{
			UCraftItemEntryUI* Owner = WeakThis.Get();
			UInventoryAsset* InventoryAsset = Cast<UInventoryAsset>(LoadedAsset);
			if (IsValid(Owner) && IsValid(InventoryAsset))
			{
				Owner->SetPrimaryDetails(
					InventoryAsset->DisplayName,
					InventoryAsset->Description,
					InventoryAsset->Icon
				);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
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


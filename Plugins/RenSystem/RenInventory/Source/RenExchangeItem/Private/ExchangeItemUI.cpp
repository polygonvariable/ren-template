// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ExchangeItemUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreCounter/Public/CounterContainer.h"
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"
#include "RCoreInventory/Public/InventoryAsset.h"
#include "RCoreInventory/Public/InventoryRecord.h"
#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryEntry.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"

#include "RCoreCounter/Public/CounterManagerInterface.h"
#include "RenExchangeItem/Public/ExchangeItemSubsystem.h"



void UExchangeItemUI::DisplayStockItems()
{
	ICounterManagerInterface* CounterManager = CounterManagerInterface.Get();
	if (!CatalogList || !CounterManager)
	{
		return;
	}

	const TMap<FPrimaryAssetId, FExchangeQuota>& Stock = Catalog.Stock;

	if (bEnableCounter)
	{
		CatalogList->ClearPayloads();

		for (const TPair<FPrimaryAssetId, FExchangeQuota>& Kv : Stock)
		{
			const FPrimaryAssetId& AssetId = Kv.Key;

			FExchangeRecord NewRecord;
			NewRecord.Quota = Kv.Value;

			const FCounterRecord* Record = CounterManager->GetCounterRecord(CounterId, AssetId.PrimaryAssetName);
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

			CatalogList->AddPayload(AssetId, FInstancedStruct::Make(NewRecord));
		}
	}

	UFilterAssetCriterion* AssetCriterion = CatalogList->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		TArray<FPrimaryAssetId> AssetIds;
		Stock.GetKeys(AssetIds);

		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	CatalogList->RefreshItems();
}

void UExchangeItemUI::DisplayRequiredItems(UObject* LoadedAsset)
{
	if (!RequiredList)
	{
		return;
	}

	RequiredList->ClearPayloads();

	const FExchangeRule* ExchangeRule = GetExchangeRule(LoadedAsset);
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule->RequiredAssets;

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
}


int UExchangeItemUI::GetExchangeQuantity() const
{
	return 1;
}

UExchangeItemSubsystem* UExchangeItemUI::GetExchangeItemSubsystem() const
{
	return nullptr;
}

const FExchangeRule* UExchangeItemUI::GetExchangeRule(UObject* Target) const
{
	return nullptr;
}


void UExchangeItemUI::ExchangeItem()
{
	UExchangeItemSubsystem* ExchangeItem = GetExchangeItemSubsystem();
	if (!IsValid(ExchangeItem))
	{
		return;
	}

	int Quantity = GetExchangeQuantity();
	if (!bEnableCounter)
	{
		ExchangeItem->Exchange(ContainerId, ActiveAssetId, Quantity);
		return;
	}

	FExchangeQuota* Quota = Catalog.Stock.Find(ActiveAssetId);
	if (!Quota)
	{
		return;
	}

	ExchangeItem->Exchange(ContainerId, ActiveAssetId, Quantity, CounterId, *Quota);
}


void UExchangeItemUI::HandleItemExchanged(bool bSuccess)
{
	if (bSuccess)
	{
		DisplayStockItems();

		if (ItemDetail) ItemDetail->RefreshDetails();
		if (RequiredList) RequiredList->RefreshItems();
	}
}

void UExchangeItemUI::CloseWidget()
{
	RemoveFromParent();
}


void UExchangeItemUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!IsValid(AssetManager) || !ItemDetail || !ExchangeButton || !RequiredList || ActiveAssetId == AssetId)
	{
		return;
	}

	ActiveAssetId = AssetId;

	ItemDetail->InitializeDetails(AssetId, Quantity);

	TWeakObjectPtr<UExchangeItemUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis](bool, UObject* LoadedObject)
		{
			UExchangeItemUI* Outer = WeakThis.Get();
			if (IsValid(Outer))
			{
				Outer->DisplayRequiredItems(LoadedObject);
			}
		};
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}


void UExchangeItemUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CatalogList)
	{
		CatalogList->bAutoRefresh = false;
		CatalogList->bEnablePayloads = true;
		CatalogList->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
		CatalogList->QueryRule.ContainerId = ContainerId;
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

void UExchangeItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();

		ICounterManagerInterface* CounterManager = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, ICounterManagerInterface>(GameInstance);
		CounterManagerInterface = TWeakInterfacePtr<ICounterManagerInterface>(CounterManager);
	}

	if (CatalogList)
	{
		CatalogList->OnItemSelected.RemoveAll(this);
		CatalogList->OnItemSelected.AddUObject(this, &UExchangeItemUI::InitializeDetails);
	}

	if (ExchangeButton)
	{
		ExchangeButton->OnClicked.RemoveAll(this);
		ExchangeButton->OnClicked.AddDynamic(this, &UExchangeItemUI::ExchangeItem);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveAll(this);
		CloseButton->OnClicked.AddDynamic(this, &UExchangeItemUI::CloseWidget);
	}

	Super::NativeConstruct();
}

void UExchangeItemUI::NativeDestruct()
{
	CounterManagerInterface.Reset();
	InventorySubsystem.Reset();

	if (CatalogList) CatalogList->OnItemSelected.RemoveAll(this);
	if (ExchangeButton) ExchangeButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}



void UExchangeItemEntryUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	TWeakObjectPtr<UExchangeItemEntryUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, Quantity](bool, UObject* LoadedAsset)
		{
			UExchangeItemEntryUI* Owner = WeakThis.Get();
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

void UExchangeItemEntryUI::SetTertiaryDetails(UInventoryEntry* Entry)
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


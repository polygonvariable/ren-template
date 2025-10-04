// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "PurchaseItemUI.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreShop/Public/ShopProviderInterface.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"
#include "RenInventory/Public/InventoryEntry.h"

#include "RenPurchaseItem/Public/PurchaseItemSubsystem.h"



void UPurchaseItemUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!AssetManager || !InventoryDetail || !RequiredCollection)
	{
		return;
	}

	InventoryDetail->InitializeDetails(AssetId, Quantity, Record);

	TWeakObjectPtr<UPurchaseItemUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis](bool, UObject* LoadedObject)
		{
			UPurchaseItemUI* Outer = WeakThis.Get();
			if (IsValid(Outer))
			{
				Outer->HandleLoadedAsset(LoadedObject);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

void UPurchaseItemUI::ResetDetails()
{

}

void UPurchaseItemUI::RefreshDetails()
{

}

int UPurchaseItemUI::GetCraftingQuantity() const
{
	return 1;
}

void UPurchaseItemUI::PurchaseItem()
{
	if (!IsValid(InventoryCollection))
	{
		return;
	}

	UPurchaseItemSubsystem* PurchaseItem = PurchaseItemSubsystem.Get();
	UInventoryEntry* SelectedItem = InventoryCollection->GetSelectedItem();
	if (!IsValid(PurchaseItem) || !IsValid(SelectedItem))
	{
		return;
	}

	int Quantity = GetCraftingQuantity();
	const FPrimaryAssetId& AssetId = SelectedItem->AssetId;

	PurchaseItem->Purchase(ContainerId, AssetId, Quantity);
}

void UPurchaseItemUI::HandleLoadedAsset(UObject* LoadedAsset)
{
	// UInventorySubsystem* Inventory = InventorySubsystem.Get();
	IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(LoadedAsset);
	if (!ShopProvider || !IsValid(RequiredCollection))
	{
		return;
	}

	TMap<FPrimaryAssetId, FInstancedStruct> Payloads;

	const FExchangeRule& ExchangeRule = ShopProvider->GetPurchaseRule();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule.RequiredAssets;

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

	UFilterGroup* FilterRule = RequiredCollection->FilterRule;
	if (IsValid(FilterRule))
	{
		UFilterAssetCriterion* AssetCriterion = FilterRule->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
		if (IsValid(AssetCriterion))
		{
			AssetCriterion->Included.Empty();
			AssetCriterion->Included.Append(AssetIds);
		}
	}

	RequiredCollection->SetPayloads(Payloads);
	RequiredCollection->DisplayItems();

	// int MaxMultiplier = Inventory->GetMaxMultiplier(ContainerId, RequiredAssets);
}

void UPurchaseItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		PurchaseItemSubsystem = GameInstance->GetSubsystem<UPurchaseItemSubsystem>();
		InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();
	}

	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.AddUObject(this, &UPurchaseItemUI::InitializeDetails);
	}

	Super::NativeConstruct();
}

void UPurchaseItemUI::NativeDestruct()
{
	PurchaseItemSubsystem.Reset();
	InventorySubsystem.Reset();

	if (InventoryCollection)
	{
		InventoryCollection->OnItemSelected.RemoveAll(this);
	}

	Super::NativeDestruct();
}


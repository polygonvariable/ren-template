// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "PurchaseItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreShop/Public/ShopProviderInterface.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UPurchaseItemSubsystem::Purchase(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogPurchaseItem, 1.0f, TEXT("AssetId, AssetManager is invalid or Quantity is <= 0"));
		return;
	}

	TWeakObjectPtr<UPurchaseItemSubsystem> WeakThis(this);

	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, ContainerId, AssetId, Quantity](bool, UObject* LoadedObject)
		{
			UPurchaseItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner) && IsValid(LoadedObject))
			{
				Owner->HandlePurchase(ContainerId, AssetId, Quantity, LoadedObject);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

bool UPurchaseItemSubsystem::HandlePurchase(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(AssetObject);
	if (!IsValid(Inventory) || !ShopProvider)
	{
		PRINT_ERROR(LogPurchaseItem, 1.0f, TEXT("InventorySubsystem, ShopProvider is invalid"));
		return false;
	}

	const FExchangeRule& ExchangeRule = ShopProvider->GetPurchaseRule();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule.RequiredAssets;

	bool bCanCraft = Inventory->ContainsItems(ContainerId, RequiredAssets, Quantity);
	if (!bCanCraft)
	{
		PRINT_ERROR(LogPurchaseItem, 1.0f, TEXT("Inventory does not contain required items"));
		return false;
	}

	bool bSuccess = Inventory->RemoveItems(ContainerId, RequiredAssets, Quantity);
	if (!bSuccess)
	{
		PRINT_ERROR(LogPurchaseItem, 1.0f, TEXT("Failed to consume required items"));
		return true;
	}

	return Inventory->AddItem(ContainerId, AssetId, Quantity);
}



void UPurchaseItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	AssetManager = UAssetManager::GetIfInitialized();
}

bool UPurchaseItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UPurchaseItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogPurchaseItem, TEXT("Initialized"));

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UPurchaseItemSubsystem::HandleGameInitialized);
	}
}

void UPurchaseItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogPurchaseItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}


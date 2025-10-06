// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Shop/PurchaseItemSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreShop/Public/ShopProviderInterface.h"

#include "RenInventory/Public/InventorySubsystem.h"



const FExchangeRule* UPurchaseItemSubsystem::GetExchangeRule(UObject* Target) const
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(Target);
	if (!IsValid(Inventory) || !ShopProvider)
	{
		return nullptr;
	}

	const FExchangeRule& ExchangeRule = ShopProvider->GetPurchaseRule();
	return &ExchangeRule;
}


bool UPurchaseItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UPurchaseItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogPurchaseItem, TEXT("Initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UPurchaseItemSubsystem::HandleGameInitialized);
}

void UPurchaseItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogPurchaseItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}


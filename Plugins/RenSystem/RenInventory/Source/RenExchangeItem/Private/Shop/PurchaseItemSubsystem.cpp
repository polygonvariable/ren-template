// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Shop/PurchaseItemSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreShop/Public/ShopProviderInterface.h"



const FExchangeRule* UPurchaseItemSubsystem::GetExchangeRule(UObject* Target) const
{
	IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(Target);
	if (!ShopProvider)
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


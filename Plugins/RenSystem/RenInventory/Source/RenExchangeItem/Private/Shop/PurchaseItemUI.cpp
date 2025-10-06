// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Shop/PurchaseItemUI.h"

// Engine Headers

// Project Headers
#include "RCoreShop/Public/ShopProviderInterface.h"
#include "RenExchangeItem/Public/Shop/PurchaseItemSubsystem.h"



int UPurchaseItemUI::GetExchangeQuantity() const
{
	return 1;
}

UExchangeItemSubsystem* UPurchaseItemUI::GetExchangeItemSubsystem() const
{
	return PurchaseItemSubsystem.Get();
}

const FExchangeRule* UPurchaseItemUI::GetExchangeRule(UObject* Target) const
{
	IShopProviderInterface* ShopProvider = Cast<IShopProviderInterface>(Target);
	if (!ShopProvider)
	{
		return nullptr;
	}

	const FExchangeRule& ExchangeRule = ShopProvider->GetPurchaseRule();
	return &ExchangeRule;
}


void UPurchaseItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UPurchaseItemSubsystem* PurchaseItem = GameInstance->GetSubsystem<UPurchaseItemSubsystem>();
		if (IsValid(PurchaseItem))
		{
			PurchaseItem->OnItemExchanged.AddUObject(this, &UPurchaseItemUI::HandleItemExchanged);
			PurchaseItemSubsystem = PurchaseItem;
		}
	}

	Super::NativeConstruct();
}

void UPurchaseItemUI::NativeDestruct()
{
	UPurchaseItemSubsystem* PurchaseItem = PurchaseItemSubsystem.Get();
	if (IsValid(PurchaseItem))
	{
		PurchaseItem->OnItemExchanged.RemoveAll(this);
	}
	PurchaseItemSubsystem.Reset();

	Super::NativeDestruct();
}


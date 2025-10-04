// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreShop/Public/ShopProviderInterface.h"

#include "RCoreInventory/Public/InventoryAsset.h"

// Generated Headers
#include "PurchasableAsset.generated.h"

// Forward Declarations



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UPurchasableAsset : public UInventoryAsset, public IShopProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FExchangeRule PurchaseRule;

	// ~ IShopProviderInterface
	RENINVENTORY_API virtual const FExchangeRule& GetPurchaseRule() const override;
	// ~ End of IShopProviderInterface

};


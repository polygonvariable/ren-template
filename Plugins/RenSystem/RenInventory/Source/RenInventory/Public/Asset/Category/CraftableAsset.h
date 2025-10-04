// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreCraft/Public/CraftProviderInterface.h"

#include "RenInventory/Public/Asset/Category/PurchasableAsset.h"

// Generated Headers
#include "CraftableAsset.generated.h"

// Forward Declarations
class UInventoryAsset;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UCraftableAsset : public UPurchasableAsset, public ICraftProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FExchangeRule CraftingRule;

	// ~ ICraftProviderInterface
	RENINVENTORY_API virtual const FExchangeRule& GetCraftingRule() const override;
	// ~ End of ICraftProviderInterface

};


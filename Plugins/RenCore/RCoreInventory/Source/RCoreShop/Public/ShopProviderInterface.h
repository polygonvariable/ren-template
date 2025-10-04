// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ShopProviderInterface.generated.h"

// Forward Declarations
struct FExchangeRule;



UINTERFACE(MinimalAPI)
class UShopProviderInterface : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class RCORESHOP_API IShopProviderInterface
{

	GENERATED_BODY()

public:

	virtual const FExchangeRule& GetPurchaseRule() const = 0;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers

// Generated Headers
#include "ShopData.generated.h"

// Module Macros
#define REN_API RSHOP_API

// Forward Declarations



/**
 *
 */
USTRUCT(BlueprintType)
struct FShopData
{

	GENERATED_BODY()

public:

	FShopData() {}
	FShopData(int InPurchaseCount);


	UPROPERTY(SaveGame)
	int PurchaseCount = 0;


	REN_API bool IsValid() const;
	REN_API void Reset();
	REN_API void Sanitize();

};



// Module Macros
#undef REN_API


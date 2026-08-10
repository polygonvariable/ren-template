// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "ShopInstance.generated.h"

// Module Macros
#define REN_API RSHOP_API


/**
 *
 */
USTRUCT(BlueprintType)
struct FShopInstance
{

	GENERATED_BODY()

public:

	FShopInstance() {}
	FShopInstance(int InPurchaseCount);


	UPROPERTY(SaveGame)
	int PurchaseCount = 0;


	REN_API bool IsValid() const;
	REN_API void Reset();
	REN_API void Sanitize();

};


// Module Macros
#undef REN_API


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/Type/Runtime/ShopInstance.h"


FShopInstance::FShopInstance(int InPurchaseCount)
{
	PurchaseCount = InPurchaseCount;
}

bool FShopInstance::IsValid() const
{
	return PurchaseCount > 0;
}

void FShopInstance::Reset()
{
	PurchaseCount = 0;
}

void FShopInstance::Sanitize()
{
	PurchaseCount = FMath::Max(0, PurchaseCount);
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/PurchasableAsset.h"

// Engine Headers

// Project Headers


const FExchangeRule& UPurchasableAsset::GetPurchaseRule() const
{
	return PurchaseRule;
}


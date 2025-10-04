// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/Category/CraftableAsset.h"

// Engine Headers

// Project Headers


const FExchangeRule& UCraftableAsset::GetCraftingRule() const
{
	return CraftingRule;
}


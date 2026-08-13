// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryFilterUI.h"

// Engine Headers

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Core/Type/AssetFilterProperty.h"
#include "Definition/FilterContext.h"
#include "Core/Type/InventoryFilterProperty.h"
#include "Filter/FilterCriterion.h"
#include "Filter/FilterGroup.h"
#include "Core/InventoryPrimaryAsset.h"
#include "Manager/RAssetManager.h"



//void UInventoryFilterUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
//{
//	if (!IsValid(AssetManager) || !IsValid(FilterRule))
//	{
//		SwitchDetail(false);
//		return;
//	}
//
//	FAssetData AssetData;
//	if (!AssetManager->GetPrimaryAssetData(GetActiveAssetId(), AssetData))
//	{
//		SwitchDetail(false);
//		return;
//	}
//
//	FName ItemType = TEXT_EMPTY;
//	FInventoryPrimaryAsset::GetType(AssetData, ItemType);
//
//	FName ItemRarity = TEXT_EMPTY;
//	FInventoryPrimaryAsset::GetRarity(AssetData, ItemRarity);
//
//	FFilterContext Context;
//	Context.SetValue(FAssetFilterProperty::AssetId, GetActiveAssetId());
//	Context.SetValue(FInventoryFilterProperty::ItemType, ItemType);
//	Context.SetValue(FInventoryFilterProperty::ItemRarity, ItemRarity);
//
//	UFilterCriterion* FilterRoot = FilterRule->CriterionRoot;
//	if (IsValid(FilterRoot))
//	{
//		SwitchDetail(FilterRoot->Evaluate(Context));
//	}
//	else
//	{
//		SwitchDetail(false);
//	}
//}


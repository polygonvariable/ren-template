// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/Fragment/AssetCompositionFragment.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/AssetRuleDefinition.h"
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"


const UAssetCollection* UAssetCompositionFragment::GetBreakdownAssets(const FGuid& InId) const
{
	if (!IsValid(BreakdownItems))
	{
		return nullptr;
	}
	return BreakdownItems->GetCollectionRule(FInstancedStruct::Make(FAssetRuleContext(InId)));
}

const UAssetCollection* UAssetCompositionFragment::GetRebuildAssets(const FGuid& InId) const
{
	if (!IsValid(RebuildItems))
	{
		return nullptr;
	}
	return RebuildItems->GetCollectionRule(FInstancedStruct::Make(FAssetRuleContext(InId)));
}


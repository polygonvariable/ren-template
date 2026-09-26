// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/AssetGroup.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/AssetRuleDefinition.h"
#include "Data/AssetCollection.h"


const UAssetCollection* UAssetGroup::GetCollectionRule() const
{
	return nullptr;
}

const UAssetCollection* UAssetGroup::GetCollectionRule(const FInstancedStruct& Context) const
{
	return nullptr;
}


const UAssetCollection* UAssetGroup_Single::GetCollectionRule() const
{
	return Collection;
}

const UAssetCollection* UAssetGroup_Single::GetCollectionRule(const FInstancedStruct& Context) const
{
	return Collection;
}


const UAssetCollection* UAssetGroup_List::GetCollectionRule() const
{
	if (Collections.Num() > 0)
	{
		return Collections.Last();
	}
	return nullptr;
}

const UAssetCollection* UAssetGroup_List::GetCollectionRule(const FInstancedStruct& Context) const
{
	const FAssetRuleContext* BaseContext = Context.GetPtr<FAssetRuleContext>();
	if (!BaseContext)
	{
		return nullptr;
	}

	const FGuid& RuleId = BaseContext->Id;
	if (!RuleId.IsValid())
	{
		const FAssetRuleContext_List* ListContext = Context.GetPtr<FAssetRuleContext_List>();
		if (ListContext)
		{
			int Index = ListContext->Index;
			if (!Collections.IsValidIndex(Index))
			{
				return nullptr;
			}

			return Collections[Index];
		}
	}

	const TObjectPtr<UAssetCollection>* FoundCollection = Collections.FindByPredicate(
		[RuleId](const UAssetCollection* Rule)
		{
			return Rule->GetCollectionId() == RuleId;
		}
	);

	if (!FoundCollection)
	{
		return nullptr;
	}

	return FoundCollection->Get();
}


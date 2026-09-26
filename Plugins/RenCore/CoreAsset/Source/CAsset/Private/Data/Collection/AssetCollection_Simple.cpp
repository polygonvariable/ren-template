// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/Collection/AssetCollection_Simple.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


const TMap<FPrimaryAssetId, FAssetDetail>& UAssetCollection_Simple::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_Simple::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAsset = Kv;
		return true;
	}
	return false;
}

bool UAssetCollection_Simple::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	const FAssetDetail* Detail = AssetList.Find(AssetId);
	if (Detail == nullptr)
	{
		return false;
	}

	OutDetail = *Detail;
	return true;
}

void UAssetCollection_Simple::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	OutAssets = AssetList;
}

void UAssetCollection_Simple::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value.Quantity);
	}
}

void UAssetCollection_Simple::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key);
	}
}

void UAssetCollection_Simple::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail& Item : AssetListEd)
	{
		TSoftObjectPtr<UCoreDataAsset> Asset = Item.DataAsset;
		UCoreDataAsset* DataAsset = Asset.LoadSynchronous();
		if (!IsValid(DataAsset))
		{
			LOG_ERROR(LogAsset, TEXT("Failed to load asset"));
			continue;
		}

		FPrimaryAssetId AssetId = DataAsset->GetPrimaryAssetId();
		if (AssetType.IsValid() && AssetType != AssetId.PrimaryAssetType)
		{
			LOG_ERROR(LogAsset, TEXT("Non-matching asset types found in collection"));
			continue;
		}

		AssetType = AssetId.PrimaryAssetType;
		AssetList.Add(AssetId, Item);
	}

#endif
}


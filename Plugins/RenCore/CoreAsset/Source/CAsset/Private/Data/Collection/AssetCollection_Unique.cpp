// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/Collection/AssetCollection_Unique.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


const TMap<FPrimaryAssetId, FAssetDetail_Unique>& UAssetCollection_Unique::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_Unique::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& AssetPair : AssetList)
	{
		const FPrimaryAssetId& AssetId = AssetPair.Key;
		const FAssetDetail& AssetDetail = AssetPair.Value;

		OutAsset = TPair<FPrimaryAssetId, FAssetDetail>(AssetId, AssetDetail);
		return true;
	}
	return false;
}

bool UAssetCollection_Unique::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	const FAssetDetail_Unique* Detail = AssetList.Find(AssetId);
	if (Detail == nullptr)
	{
		return false;
	}
	OutDetail = *Detail;
	return true;
}

void UAssetCollection_Unique::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value);
	}
}

void UAssetCollection_Unique::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value.Quantity);
	}
}

void UAssetCollection_Unique::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail_Unique>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key);
	}
}

void UAssetCollection_Unique::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail_Unique& Item : AssetListEd)
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


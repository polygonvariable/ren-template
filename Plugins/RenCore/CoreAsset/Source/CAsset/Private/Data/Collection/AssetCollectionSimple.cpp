// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/Collection/AssetCollectionSimple.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Data/FragmentedDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


const TMap<FPrimaryAssetId, FAssetDetail>& UAssetCollectionSimple::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollectionSimple::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAsset = Kv;
		return true;
	}
	return false;
}

bool UAssetCollectionSimple::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	const FAssetDetail* Detail = AssetList.Find(AssetId);
	if (Detail == nullptr)
	{
		return false;
	}

	OutDetail = *Detail;
	return true;
}

void UAssetCollectionSimple::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	OutAssets = AssetList;
}

void UAssetCollectionSimple::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key, Kv.Value.Quantity);
	}
}

void UAssetCollectionSimple::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<FPrimaryAssetId, FAssetDetail>& Kv : AssetList)
	{
		OutAssets.Add(Kv.Key);
	}
}

void UAssetCollectionSimple::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail& Item : AssetListEd)
	{
		TSoftObjectPtr<UFragmentedDataAsset> Asset = Item.DataAsset;
		UFragmentedDataAsset* DataAsset = Asset.LoadSynchronous();
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


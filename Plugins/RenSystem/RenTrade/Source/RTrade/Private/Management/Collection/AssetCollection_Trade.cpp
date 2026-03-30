// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Management/Collection/AssetCollection_Trade.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"



const TMap<UCoreDataAsset*, FAssetDetail_Trade>& UAssetCollection_Trade::GetAssetList() const
{
	return AssetList;
}

bool UAssetCollection_Trade::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail_Trade& OutDetail) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		if (AssetId == Asset->GetPrimaryAssetId())
		{
			OutDetail = Kv.Value;
			return true;
		}
	}
	return false;
}

bool UAssetCollection_Trade::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Kv.Key->GetPrimaryAssetId();
		FAssetDetail AssetDetail = Kv.Value;

		OutAsset = TPair<FPrimaryAssetId, FAssetDetail>(AssetId, AssetDetail);
		return true;
	}
	return false;
}

bool UAssetCollection_Trade::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		if (AssetId == Asset->GetPrimaryAssetId())
		{
			OutDetail = Kv.Value;
			return true;
		}
	}
	return false;
}

void UAssetCollection_Trade::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId, Kv.Value);
	}
}

void UAssetCollection_Trade::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId, Kv.Value.Quantity);
	}
}

void UAssetCollection_Trade::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& Kv : AssetList)
	{
		const UCoreDataAsset* Asset = Kv.Key;
		if (!IsValid(Asset))
		{
			continue;
		}

		FPrimaryAssetId AssetId = Asset->GetPrimaryAssetId();
		OutAssets.Add(AssetId);
	}
}

void UAssetCollection_Trade::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FAssetDetail_Trade& Item : AssetListEd)
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
		AssetList.Add(DataAsset, Item);
	}

#endif
}


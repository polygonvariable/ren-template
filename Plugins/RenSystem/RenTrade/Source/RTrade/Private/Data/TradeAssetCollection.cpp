// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/TradeAssetCollection.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Data/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


const TMap<UCoreDataAsset*, FTradeAssetDetail>& UTradeAssetCollection::GetAssetList() const
{
	return AssetList;
}

bool UTradeAssetCollection::GetAssetDetail(const FPrimaryAssetId& AssetId, FTradeAssetDetail& OutDetail) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

bool UTradeAssetCollection::GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

bool UTradeAssetCollection::GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

void UTradeAssetCollection::GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

void UTradeAssetCollection::GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

void UTradeAssetCollection::GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& Kv : AssetList)
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

void UTradeAssetCollection::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR

	AssetType = FPrimaryAssetType();
	AssetList.Empty();

	for (const FTradeAssetDetail& Item : AssetListEd)
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


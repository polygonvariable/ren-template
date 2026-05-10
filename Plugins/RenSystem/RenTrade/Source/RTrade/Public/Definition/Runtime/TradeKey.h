// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "TradeKey.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FTradeKey
{

	GENERATED_BODY()

public:

	FTradeKey() {}
	FTradeKey(FPrimaryAssetId InTradeAssetId, FGuid InTradeCollectionId, FPrimaryAssetId InTargetAssetId)
	{
		TradeAssetId = InTradeAssetId;
		TradeCollectionId = InTradeCollectionId;
		TargetAssetId = InTargetAssetId;
	}


	UPROPERTY(SaveGame)
	FPrimaryAssetId TradeAssetId;

	UPROPERTY(SaveGame)
	FGuid TradeCollectionId;

	UPROPERTY(SaveGame)
	FPrimaryAssetId TargetAssetId;


	friend inline bool operator == (const FTradeKey& A, const FTradeKey& B)
	{
		return
			A.TradeAssetId == B.TradeAssetId &&
			A.TradeCollectionId == B.TradeCollectionId &&
			A.TargetAssetId == B.TargetAssetId;
	}

	friend inline uint32 GetTypeHash(const FTradeKey& Item)
	{
		uint32 Hash = GetTypeHash(Item.TradeAssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(Item.TradeCollectionId));
		Hash = HashCombineFast(Hash, GetTypeHash(Item.TargetAssetId));
		return Hash;
	}

};


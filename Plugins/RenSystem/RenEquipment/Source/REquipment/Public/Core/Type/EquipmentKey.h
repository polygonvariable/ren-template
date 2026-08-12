// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentKey.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FEquipmentKey
{

	GENERATED_BODY()

public:

	FEquipmentKey() {};
	FEquipmentKey(FPrimaryAssetId InAssetId, FGuid InAssetInstanceId) : AssetId(InAssetId), AssetInstanceId(InAssetInstanceId) {};


	UPROPERTY(EditAnywhere, SaveGame)
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere, SaveGame)
	FGuid AssetInstanceId;


	friend inline bool operator == (const FEquipmentKey& A, const FEquipmentKey& B)
	{
		return A.AssetId == B.AssetId && A.AssetInstanceId == B.AssetInstanceId;
	}

	friend inline uint32 GetTypeHash(const FEquipmentKey& A)
	{
		return HashCombine(GetTypeHash(A.AssetId), GetTypeHash(A.AssetInstanceId));
	}

};


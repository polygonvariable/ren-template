// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSlotId.h"

// Generated Headers
#include "EquipmentSpawnData.generated.h"


/**
 *
 */
USTRUCT()
struct FEquipmentInitializationData
{

	GENERATED_BODY()

public:

	FEquipmentInitializationData() {};
	FEquipmentInitializationData(FPrimaryAssetId InAssetId, FGuid InAssetInstanceId, FEquipmentSlotId InSlotId) : AssetId(InAssetId), AssetInstanceId(InAssetInstanceId), SlotId(InSlotId) {};


	UPROPERTY(EditAnywhere)
	FPrimaryAssetId AssetId;

	UPROPERTY()
	FGuid AssetInstanceId;

	UPROPERTY(EditAnywhere)
	FEquipmentSlotId SlotId;


	bool IsValid() const
	{
		return AssetId.IsValid() && SlotId.IsValid();
	}

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		AssetInstanceId.Invalidate();
		SlotId.Reset();
	}

	friend inline bool operator == (const FEquipmentInitializationData& A, const FEquipmentInitializationData& B)
	{
		return A.AssetId == B.AssetId && A.AssetInstanceId == B.AssetInstanceId && A.SlotId == B.SlotId;
	}

	friend inline uint32 GetTypeHash(const FEquipmentInitializationData& A)
	{
		uint32 Hash = GetTypeHash(A.AssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(A.SlotId));
		Hash = HashCombineFast(Hash, GetTypeHash(A.AssetInstanceId));
		return Hash;
	}

};


/**
 *
 */
USTRUCT()
struct FEquipmentInitializationDataList
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentInitializationData> EquipmentList;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSlotDefinition.h"

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
	FEquipmentInitializationData(FPrimaryAssetId InAssetId, FGuid InAssetInstanceId, FEquipmentSlotDefinition InSlotDefinition) : AssetId(InAssetId), AssetInstanceId(InAssetInstanceId), SlotDefinition(InSlotDefinition) {};


	UPROPERTY(EditAnywhere)
	FPrimaryAssetId AssetId;

	UPROPERTY()
	FGuid AssetInstanceId;

	UPROPERTY(EditAnywhere)
	FEquipmentSlotDefinition SlotDefinition;


	bool IsValid() const
	{
		return AssetId.IsValid() && SlotDefinition.IsValid();
	}

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		AssetInstanceId.Invalidate();
		SlotDefinition.Reset();
	}

	friend inline bool operator == (const FEquipmentInitializationData& A, const FEquipmentInitializationData& B)
	{
		return A.AssetId == B.AssetId && A.AssetInstanceId == B.AssetInstanceId && A.SlotDefinition == B.SlotDefinition;
	}

	friend inline uint32 GetTypeHash(const FEquipmentInitializationData& A)
	{
		uint32 Hash = GetTypeHash(A.AssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(A.SlotDefinition));
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


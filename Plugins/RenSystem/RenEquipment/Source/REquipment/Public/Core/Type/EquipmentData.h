// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/QueryType.h"

// Generated Headers
#include "EquipmentData.generated.h"


/**
 *
 */
USTRUCT()
struct FEquipmentData
{

	GENERATED_BODY()

public:

	FEquipmentData() {};


	UPROPERTY(EditAnywhere)
	FPrimaryAssetId AssetId;

	UPROPERTY(VisibleAnywhere)
	FGuid AssetInstanceId;

	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere)
	int SlotId = 10;

	UPROPERTY(VisibleAnywhere)
	EDataSource SourceType = EDataSource::Static;

	bool IsValid() const
	{
		return AssetId.IsValid() && CategoryTag.IsValid();
	}

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		AssetInstanceId.Invalidate();
		SlotId = -1;
		CategoryTag = FGameplayTag();
		SourceType = EDataSource::Static;
	}

	friend inline bool operator == (const FEquipmentData& A, const FEquipmentData& B)
	{
		return A.AssetId == B.AssetId && A.AssetInstanceId == B.AssetInstanceId && A.CategoryTag == B.CategoryTag && A.SlotId == B.SlotId && A.SourceType == B.SourceType;
	}

	friend inline uint32 GetTypeHash(const FEquipmentData& A)
	{
		uint32 Hash = GetTypeHash(A.AssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(A.CategoryTag));
		Hash = HashCombineFast(Hash, GetTypeHash(A.SlotId));
		Hash = HashCombineFast(Hash, GetTypeHash(A.AssetInstanceId));
		Hash = HashCombineFast(Hash, GetTypeHash(A.SourceType));
		return Hash;
	}

};


/**
 *
 */
USTRUCT()
struct FEquipmentDataList
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentData> EquipmentList;

};


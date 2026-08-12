// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/AssetQuerySource.h"
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

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;

	UPROPERTY(VisibleAnywhere)
	FGuid EquipmentId;

	UPROPERTY(VisibleAnywhere)
	EDataSource SourceType = EDataSource::Static;

	bool IsValid() const
	{
		return AssetId.IsValid() && EquipmentSlot.IsValid();
	}

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		EquipmentId.Invalidate();
		EquipmentSlot = FGameplayTag();
		SourceType = EDataSource::Static;
	}

	friend inline bool operator == (const FEquipmentData& A, const FEquipmentData& B)
	{
		return A.AssetId == B.AssetId && A.EquipmentSlot == B.EquipmentSlot && A.EquipmentId == B.EquipmentId && A.SourceType == B.SourceType;
	}

	friend inline uint32 GetTypeHash(const FEquipmentData& A)
	{
		uint32 Hash = GetTypeHash(A.AssetId);
		Hash = HashCombineFast(Hash, GetTypeHash(A.EquipmentSlot));
		Hash = HashCombineFast(Hash, GetTypeHash(A.EquipmentId));
		Hash = HashCombineFast(Hash, GetTypeHash(A.SourceType));
		return Hash;
	}

};

USTRUCT()
struct FEquipmentDataList
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentData> EquipmentList;

};


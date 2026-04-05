// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/AssetQuerySource.h"

// Generated Headers
#include "EquipmentSpawnData.generated.h"


USTRUCT(BlueprintType)
struct FEquipmentData
{

	GENERATED_BODY()

public:

	FEquipmentData() {};

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;

	UPROPERTY(EditAnywhere)
	FGuid EquipmentId;

	UPROPERTY(EditAnywhere)
	EAssetQuerySource SourceType = EAssetQuerySource::Asset;

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		EquipmentSlot = FGameplayTag();
		EquipmentId.Invalidate();
	}

};

USTRUCT(BlueprintType)
struct FEquipmentDataList
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentData> EquipmentList;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Definition/AscensionData.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "EquipmentSpawnData.generated.h"

// Forward Declarations
class UCoreDataAsset;


USTRUCT(BlueprintType)
struct FEquipmentSpawnData
{

	GENERATED_BODY()

public:

	FEquipmentSpawnData() {};

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UCoreDataAsset> DataAsset;

#endif

	UPROPERTY()
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere)
	FGameplayTag EquipmentSlot;

	UPROPERTY(EditAnywhere)
	FGuid EquipmentId;

	UPROPERTY(EditAnywhere)
	FAscensionData AscensionData;


	void Reset()
	{
#if WITH_EDITOR
		DataAsset.Reset();
#endif
		EquipmentId.Invalidate();
		AscensionData.Reset();
	}

};


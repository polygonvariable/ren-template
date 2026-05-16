// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagAssetInterface.h"
#include "InstancedStruct.h"

// Generated Headers
#include "CharacterInitializationData.generated.h"

/*
 *
 */
USTRUCT()
struct FCharacterInitializationData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Meta = (AllowedTypes = "Asset.Character"))
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> Attributes;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FInstancedStruct> Metadata;

	bool IsValid() const
	{
		return AssetId.IsValid();
	}

};


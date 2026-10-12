// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "ComponentTemplateDefinition.h"

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

	FCharacterInitializationData() {};
	FCharacterInitializationData(FPrimaryAssetId InAssetId) : AssetId(InAssetId) {};

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Character"))
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere, meta = (Categories = "Data.Attribute"))
	TMap<FGameplayTag, float> Attributes;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FInstancedStruct> Metadata;

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FComponentTemplateDefinition>> Components;

	bool IsValid() const
	{
		return AssetId.IsValid();
	}

	void Reset()
	{
		AssetId = FPrimaryAssetId();
		Attributes.Empty();
		Metadata.Empty();
		Components.Empty();
	}

};


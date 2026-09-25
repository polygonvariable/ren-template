// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Data/Asset/MetadataAsset.h"

// Generated Headers
#include "GameplayModeAsset.generated.h"


/**
 * 
 */
UCLASS(MinimalAPI)
class UGameplayModeAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer GameplayModeTag;

};


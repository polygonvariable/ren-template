// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayModMagnitudeCalculation.h"

// Project Headers

// Generated Headers
#include "MapMagnitudeByTags.generated.h"



/**
 *
 */
UCLASS(Abstract, DisplayName="Map Magnitude By Tags", Within=GameplayEffect)
class UMapMagnitudeByTagsEffectComponent : public UGameplayModMagnitudeCalculation
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, float> MagnitudeByTag;

public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};


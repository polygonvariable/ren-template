// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"

// Project Headers

// Generated Headers
#include "LevelCalculation.generated.h"



/**
 * TODO:
 * Remove this class as its no longer needed.
 * 
 * Use override in GameplayEffect to set the new value based on the level attribute.
 * This calculation will return the level attribute value.
 */
UCLASS()
class ULevelMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{

	GENERATED_BODY()

public:

	ULevelMagnitudeCalculation();

	FGameplayEffectAttributeCaptureDefinition LevelCaptureDef;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};


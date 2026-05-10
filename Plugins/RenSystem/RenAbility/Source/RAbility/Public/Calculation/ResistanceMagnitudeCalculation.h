// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"

// Project Headers

// Generated Headers
#include "ResistanceMagnitudeCalculation.generated.h"



/**
 * 
 * Calculate value based on Resistance Attributes.
 * 
 * In Gameplay Effect use custom calculation class.
 * - Coefficient: 1.0 (since (Coefficient * CalculatedValue) and CalculatedValue is our final resistance value, we need to multiply by 1.0, so it can be subtracted to initial damage(Pre Multiplier))
 * - Pre Multiplier: 100.0 (the actual damage value)
 * - Post Multiplier: 0.0 (value added after final damage is calculated)
 * 
 */
UCLASS(Abstract, DisplayName = "Resistance Modifier", Within = GameplayEffect)
class UResistanceMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{

	GENERATED_BODY()

public:

	// ~ UGameplayModMagnitudeCalculation
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
	// ~ End of UGameplayModMagnitudeCalculation

};


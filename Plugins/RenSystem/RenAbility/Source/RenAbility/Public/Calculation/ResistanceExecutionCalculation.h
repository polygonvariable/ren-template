// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"

// Project Headers

// Generated Headers
#include "ResistanceExecutionCalculation.generated.h"



/**
 * 
 * Calculate value based on Resistance Attributes and apply it to TargetAttribute.
 * 
 * 
 */
UCLASS(Abstract, DisplayName = "Resistance Calculation", Within = GameplayEffect)
class UResistanceExecutionCalculation : public UGameplayEffectExecutionCalculation
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute TargetAttribute;

	// ~ UGameplayEffectExecutionCalculation
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
	// ~ End of UGameplayEffectExecutionCalculation

};


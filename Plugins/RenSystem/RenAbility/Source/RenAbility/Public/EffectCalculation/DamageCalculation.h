// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GameplayEffectUIData.h"

// Project Headers

// Generated Headers
#include "DamageCalculation.generated.h"


/**
 *
 */
UCLASS()
class UAggregateDamageMMC : public UGameplayModMagnitudeCalculation
{

	GENERATED_BODY()

public:

	UAggregateDamageMMC();

	FGameplayEffectAttributeCaptureDefinition DamageCaptureDef;

protected:

	UAbilitySystemComponent* GetSourceASC(const FGameplayEffectContext* Context) const;
	UAbilitySystemComponent* GetTargetASC(const FGameplayEffectContext* Context) const;

	float GetAggregateValue(UAbilitySystemComponent*, const FGameplayAttribute& Attribute) const;

public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};



/**
 *
 */
UCLASS(Abstract)
class UDamageMagnitudeCalculation : public UGameplayModMagnitudeCalculation
{

	GENERATED_BODY()

public:

	FGameplayEffectAttributeCaptureDefinition DamageCaptureDef;
	FGameplayEffectAttributeCaptureDefinition DefenseCaptureDef;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:

	void InitializeAttributes(FGameplayAttribute DamageAttribute, FGameplayAttribute DefenseAttribute);

};


/**
 *
 */
UCLASS()
class UPhysicalDamageMagnitudeCalculation : public UDamageMagnitudeCalculation
{

	GENERATED_BODY()

public:

	UPhysicalDamageMagnitudeCalculation();

};


/**
 *
 */
UCLASS()
class UMagicalDamageMagnitudeCalculation : public UDamageMagnitudeCalculation
{

	GENERATED_BODY()

public:

	UMagicalDamageMagnitudeCalculation();

};


/**
 *
 */
UCLASS()
class UCounterDamageCalculation : public UGameplayEffectExecutionCalculation
{

	GENERATED_BODY()

};




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


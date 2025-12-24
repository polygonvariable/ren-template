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


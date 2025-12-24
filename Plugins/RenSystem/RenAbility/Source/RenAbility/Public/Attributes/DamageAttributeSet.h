// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

// Project Headers
#include "AbilityMacro.h"

// Generated Headers
#include "DamageAttributeSet.generated.h"


/**
 * 
 */
UCLASS()
class RENABILITY_API UDamageAttributeSet : public UAttributeSet
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData PhysicalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MagicalDamage;

	

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

public:

	ATTRIBUTE_ACCESSORS(UDamageAttributeSet, PhysicalDamage)
	ATTRIBUTE_ACCESSORS(UDamageAttributeSet, MagicalDamage)

};


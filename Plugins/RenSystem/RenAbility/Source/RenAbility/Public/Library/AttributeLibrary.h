// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "AttributeSet.h"

// Generated Headers
#include "AttributeLibrary.generated.h"



/**
 * 
 */
UCLASS()
class RENABILITY_API UAttributeLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	static float GetAggregateAttributeValue(UAbilitySystemComponent* TargetASC, const FGameplayAttribute& Attribute, const FGameplayEffectContext* Context);

};


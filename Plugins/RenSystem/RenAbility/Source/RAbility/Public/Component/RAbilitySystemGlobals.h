// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"

// Project Headers

// Generated Headers
#include "RAbilitySystemGlobals.generated.h"

// Forward Declarations
struct FGameplayEffectContext;



/**
 * 
 * 
 */
UCLASS(Config = Game)
class URAbilitySystemGlobals : public UAbilitySystemGlobals
{

	GENERATED_BODY()


public:


	// ~ UAbilitySystemGlobals
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	// ~ End of UAbilitySystemGlobals

};


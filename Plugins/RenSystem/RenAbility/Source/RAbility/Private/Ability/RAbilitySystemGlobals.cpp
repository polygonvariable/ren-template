// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/RAbilitySystemGlobals.h"

// Project Headers
#include "Ability/RGameplayEffectContext.h"


FGameplayEffectContext* URAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRGameplayEffectContext();
}


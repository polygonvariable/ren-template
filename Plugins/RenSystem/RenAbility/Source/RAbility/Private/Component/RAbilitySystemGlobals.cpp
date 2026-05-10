// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Component/RAbilitySystemGlobals.h"

// Engine Headers

// Project Headers
#include "Component/RGameplayEffectContext.h"



FGameplayEffectContext* URAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRGameplayEffectContext();
}


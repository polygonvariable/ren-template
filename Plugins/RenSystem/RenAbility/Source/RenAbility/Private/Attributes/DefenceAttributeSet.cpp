// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/DefenceAttributeSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Project Headers



void UDefenceAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
}


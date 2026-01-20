// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/DamageSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers



void UDamageSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
}

void UDamageSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDamageSet, Physical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDamageSet, Elemental, COND_None, REPNOTIFY_Always);
}

void UDamageSet::OnRep_Physical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDamageSet, Physical, OldValue);
}

void UDamageSet::OnRep_Elemental(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDamageSet, Elemental, OldValue);
}


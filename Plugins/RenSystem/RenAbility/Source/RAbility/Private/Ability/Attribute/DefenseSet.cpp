// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/Attribute/DefenseSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"


void UDefenseSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
}

void UDefenseSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDefenseSet, Physical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefenseSet, Elemental, COND_None, REPNOTIFY_Always);
}

void UDefenseSet::OnRep_Physical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenseSet, Physical, OldValue);
}

void UDefenseSet::OnRep_Elemental(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenseSet, Elemental, OldValue);
}



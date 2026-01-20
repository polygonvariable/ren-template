// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/DefenceSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Project Headers



void UDefenceSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
}

void UDefenceSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDefenceSet, Physical, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDefenceSet, Elemental, COND_None, REPNOTIFY_Always);
}

void UDefenceSet::OnRep_Physical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenceSet, Physical, OldValue);
}

void UDefenceSet::OnRep_Elemental(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDefenceSet, Elemental, OldValue);
}



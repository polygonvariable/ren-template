// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/ManaSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "RCoreTag/Public/AttributeTags.h"



void UManaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UManaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetManaAttribute())
	{
		float NewValue = FMath::Clamp(GetMana(), 0.0f, GetMaxMana());
		SetMana(NewValue);
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		float NewValue = FMath::Max(GetMaxMana(), 1.0f);
		SetMaxMana(NewValue);
	}
}

void UManaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UManaSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UManaSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UManaSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManaSet, Mana, OldValue);
}

void UManaSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UManaSet, MaxMana, OldValue);
}


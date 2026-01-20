// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/StaminaSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "InstancedStruct.h"
#include "Net/UnrealNetwork.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"



void UStaminaSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxStaminaAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.0f);
	}
}

void UStaminaSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& Attribute = Data.EvaluatedData.Attribute;

	if (Attribute == GetStaminaAttribute())
	{
		float NewValue = FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina());
		SetStamina(NewValue);
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		float NewValue = FMath::Max(GetMaxStamina(), 1.0f);
		SetMaxStamina(NewValue);
	}

	/*
	if (GetStamina() <= 0.0f)
	{
		OnStaminaExhausted.Broadcast();
	}
	if (GetStamina() >= GetMaxStamina())
	{
		OnStaminaRefilled.Broadcast();
	}
	*/
}

void UStaminaSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UStaminaSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaSet, Stamina, OldValue);
}

void UStaminaSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaSet, MaxStamina, OldValue);
}


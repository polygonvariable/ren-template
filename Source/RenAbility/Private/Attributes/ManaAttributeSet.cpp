// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Attributes/ManaAttributeSet.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "NativeGameplayTags.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenCore/Public/Tag/GameTags.h"



void UManaAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetManaMax());
	}
	else if (Attribute == GetManaMaxAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FLT_MAX);
	}
}

void UManaAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetManaMaxAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetManaMax()));

		if (GetMana() <= 0.0f)
		{
			if (UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent())
			{
				AbilityComponent->AddLooseGameplayTag(TAG_Attribute_State_Mana_Depleted);
				AbilityComponent->RemoveLooseGameplayTag(TAG_Attribute_State_Mana_Available);
			}
		}
	}
}


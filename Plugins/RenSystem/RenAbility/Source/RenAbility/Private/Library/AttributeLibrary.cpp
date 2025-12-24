// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AttributeLibrary.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Component/RAbilitySystemComponent.h"


float UAttributeLibrary::GetAggregateAttributeValue(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, const FGameplayEffectContext* Context)
{
	if (!IsValid(ASC))
	{
		return 0.0f;
	}

	float AggregateValue = 0.0f;

	if (URAbilitySystemComponent* CastedInstigatorASC = Cast<URAbilitySystemComponent>(ASC))
	{
		AggregateValue = CastedInstigatorASC->GetAggregatedNumericAttribute(Attribute);
	}
	else
	{
		const TArray<TWeakObjectPtr<AActor>>& ContextActors = Context->GetActors();
		for (TWeakObjectPtr<AActor> ContextActor : ContextActors)
		{
			if (UAbilitySystemComponent* ContextASC = ContextActor->GetComponentByClass<UAbilitySystemComponent>())
			{
				AggregateValue += ContextASC->GetNumericAttribute(Attribute);
			}
		}
	}

	return 0.0f;
}

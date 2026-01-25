// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EffectCalculation/DamageCalculation.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"

#include "Attributes/DefenceSet.h"
#include "Attributes/DamageSet.h"
#include "Attributes/LevelSet.h"
#include "Attributes/HealthSet.h"
#include "Component/RAbilitySystemComponent.h"
#include "Library/AttributeLibrary.h"



float UDamageMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Defense = 0;
	GetCapturedAttributeMagnitude(DefenseCaptureDef, Spec, EvaluateParameters, Defense);

	float Damage = 0;
	GetCapturedAttributeMagnitude(DamageCaptureDef, Spec, EvaluateParameters, Damage);

	return FMath::RoundToInt(
		FMath::Max(0.0f, Damage - Defense)
	);
}

void UDamageMagnitudeCalculation::InitializeAttributes(FGameplayAttribute DamageAttribute, FGameplayAttribute DefenseAttribute)
{
	if (!DamageAttribute.IsValid() || !DefenseAttribute.IsValid())
	{
		PRINT_ERROR(LogTemp, 5.0f, TEXT("DamageAttribute or DefenseAttribute is not valid"));
		return;
	}

	DefenseCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenseCaptureDef.AttributeToCapture = DefenseAttribute;
	DefenseCaptureDef.bSnapshot = false;

	DamageCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DamageCaptureDef.AttributeToCapture = DamageAttribute;
	DamageCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DefenseCaptureDef);
	RelevantAttributesToCapture.Add(DamageCaptureDef);
}



UPhysicalDamageMagnitudeCalculation::UPhysicalDamageMagnitudeCalculation()
{
	InitializeAttributes(
		UDamageSet::GetPhysicalAttribute(),
		UDefenceSet::GetPhysicalAttribute()
	);
}

UMagicalDamageMagnitudeCalculation::UMagicalDamageMagnitudeCalculation()
{
	InitializeAttributes(
		UDamageSet::GetElementalAttribute(),
		UDefenceSet::GetElementalAttribute()
	);
}



UAggregateDamageMMC::UAggregateDamageMMC()
{
	DamageCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DamageCaptureDef.AttributeToCapture = UDamageSet::GetPhysicalAttribute();
	DamageCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DamageCaptureDef);
}

UAbilitySystemComponent* UAggregateDamageMMC::GetSourceASC(const FGameplayEffectContext* Context) const
{
	return Context->GetOriginalInstigatorAbilitySystemComponent();
}

UAbilitySystemComponent* UAggregateDamageMMC::GetTargetASC(const FGameplayEffectContext* Context) const
{
	TArray<TWeakObjectPtr<AActor>> TargetActors = Context->GetActors();
	if (TargetActors.IsValidIndex(0))
	{
		return TargetActors[0]->GetComponentByClass<UAbilitySystemComponent>();
	}
	return nullptr;
}

float UAggregateDamageMMC::GetAggregateValue(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute) const
{
	float AggregateValue = 0.0f;
	
	if (URAbilitySystemComponent* CastedASC = Cast<URAbilitySystemComponent>(ASC))
	{
		//AggregateValue = CastedASC->GetAggregatedNumericAttribute(Attribute);
	}
	else
	{
		//AggregateValue = CastedASC->GetNumericAttribute(Attribute);
	}

	return AggregateValue;

}

float UAggregateDamageMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayEffectContextHandle& ContextHandle = Spec.GetEffectContext();
	if (!ContextHandle.IsValid()) return 0.0f;

	const FGameplayEffectContext* Context = ContextHandle.Get();
	if (!Context) return 0.0f;

	float SourceDamage = GetAggregateValue(GetSourceASC(Context), UDamageSet::GetPhysicalAttribute());
	float TargetDefence = GetAggregateValue(GetTargetASC(Context), UDefenceSet::GetPhysicalAttribute());

	return FMath::RoundToInt(
		FMath::Max(0.0f, SourceDamage - TargetDefence)
	);
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Calculation/ResistanceExecutionCalculation.h"

// Engine Headers
#include "Component/RGameplayEffectContext.h"

// Project Headers



void UResistanceExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	//FGameplayTag TemporaryTag = ValidTransientAggregatorIdentifiers.First();

	//if (TargetAttribute.IsValid() && TemporaryTag.IsValid())
	//{
	//	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	//	
	//	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	//	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//	FAggregatorEvaluateParameters EvaluateParameters;
	//	EvaluateParameters.SourceTags = SourceTags;
	//	EvaluateParameters.TargetTags = TargetTags;

	//	float CapturedValue = 0;

	//	const TArray<FGameplayEffectAttributeCaptureDefinition>& Attributes = RelevantAttributesToCapture;
	//	for (const FGameplayEffectAttributeCaptureDefinition& Attribute : Attributes)
	//	{
	//		float Value = 0;
	//		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attribute, EvaluateParameters, Value);

	//		CapturedValue += Value;
	//	}

	//	float TargetValue = 0.0f;
	//	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(TemporaryTag, EvaluateParameters, TargetValue);

	//	float NewValue = FMath::Max(TargetValue - CapturedValue, 0);

	//	if (NewValue > 0.0f)
	//	{
	//		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttribute, EGameplayModOp::Additive, NewValue));
	//	}
	//}

#endif
}






void UAttributeInitializationExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	int Level = GetLevel(Spec);

	for (const FAttributeInitializationData& Item : InitializationData)
	{
		const FGameplayTag& Tag = Item.Tag;
		const FGameplayAttribute& Attribute = Item.Attribute;

		if (!Tag.IsValid() || !Attribute.IsValid())
		{
			continue;
		}

		float Value = Spec.GetSetByCallerMagnitude(Tag, false, 0.0f);
		float ScaledValue = Value * Level;

		if (ScaledValue > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(Attribute, EGameplayModOp::Override, ScaledValue));
		}
	}

#endif
}

int UAttributeInitializationExecutionCalculation::GetLevel(const FGameplayEffectSpec& Spec) const
{
	if (!bApplyLevel)
	{
		return 1;
	}
	return FMath::Clamp(Spec.GetLevel(), 1, MaxLevel);
}


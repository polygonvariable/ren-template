// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Calculation/ResistanceExecutionCalculation.h"

// Engine Headers

// Project Headers



void UResistanceExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	FGameplayTag TemporaryTag = ValidTransientAggregatorIdentifiers.First();

	if (TargetAttribute.IsValid() && TemporaryTag.IsValid())
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
		const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

		FAggregatorEvaluateParameters EvaluateParameters;
		EvaluateParameters.SourceTags = SourceTags;
		EvaluateParameters.TargetTags = TargetTags;

		float CapturedValue = 0;

		const TArray<FGameplayEffectAttributeCaptureDefinition>& Attributes = RelevantAttributesToCapture;
		for (const FGameplayEffectAttributeCaptureDefinition& Attribute : Attributes)
		{
			float Value = 0;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Attribute, EvaluateParameters, Value);

			CapturedValue += Value;
		}

		float TargetValue = 0.0f;
		ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(TemporaryTag, EvaluateParameters, TargetValue);

		float NewValue = FMath::Max(TargetValue - CapturedValue, 0);

		if (NewValue > 0.0f)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(TargetAttribute, EGameplayModOp::Additive, NewValue));
		}
	}

#endif
}


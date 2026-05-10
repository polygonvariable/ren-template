// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Calculation/ResistanceMagnitudeCalculation.h"

// Engine Headers

// Project Headers



float UResistanceMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Resistance = 0;

	const TArray<FGameplayEffectAttributeCaptureDefinition>& Attributes = RelevantAttributesToCapture;
	for (const FGameplayEffectAttributeCaptureDefinition& Attribute : Attributes)
	{
		float AttributeValue = 0;
		GetCapturedAttributeMagnitude(Attribute, Spec, EvaluateParameters, AttributeValue);

		Resistance += AttributeValue;
	}

	return Resistance * -1.0f;
}


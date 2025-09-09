// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EffectCalculation/LevelCalculation.h"

// Engine Headers
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

// Project Headers
#include "RCoreLibrary/Public/LogMacro.h"
#include "RenCore/Public/Tag/GameTags.h"

#include "Attributes/LevelAttributeSet.h"



ULevelMagnitudeCalculation::ULevelMagnitudeCalculation()
{
	LevelCaptureDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	LevelCaptureDef.AttributeToCapture = ULevelAttributeSet::GetLevelAttribute();
	LevelCaptureDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(LevelCaptureDef);
}

float ULevelMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Level = 0;
	GetCapturedAttributeMagnitude(LevelCaptureDef, Spec, EvaluateParameters, Level);

	return FMath::RoundToInt(
		FMath::Max(1.0f, Level)
	);
}

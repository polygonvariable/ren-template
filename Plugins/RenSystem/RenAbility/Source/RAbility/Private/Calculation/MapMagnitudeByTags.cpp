// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Calculation/MapMagnitudeByTags.h"

// Engine Headers

// Project Headers



float UMapMagnitudeByTagsEffectComponent::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float NewMagnitude = 0.0f;

	const FTagContainerAggregator& ContainerAggregator = Spec.CapturedTargetTags;
	const FGameplayTagContainer& TagContainer = ContainerAggregator.GetActorTags();

	for (const TPair<FGameplayTag, float>& Tag : MagnitudeByTag)
	{
		if (TagContainer.HasTag(Tag.Key))
		{
			NewMagnitude = Tag.Value;
			break;
		}
	}

	return NewMagnitude;
}


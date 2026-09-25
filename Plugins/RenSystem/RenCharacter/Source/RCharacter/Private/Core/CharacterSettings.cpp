// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/CharacterSettings.h"


UCharacterSettings::UCharacterSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	DataHealthTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.Health"));
	DataMaxHealthTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.MaxHealth"));

	DataLevelTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.Level"));

	DataPhysicalDamageTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.PhysicalDamage"));
	DataPhysicalDefenseTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.PhysicalDefense"));

	DataElementalDamageTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.ElementalDamage"));
	DataElementalDefenseTag = FGameplayTag::RequestGameplayTag(TEXT("Data.Attribute.ElementalDefense"));

	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.None")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Walking")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.NavWalking")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Falling")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Swimming")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Flying")));
	MovementModeTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Custom")));
}

FGameplayTag UCharacterSettings::ConvertMovementModeToTag(EMovementMode MovementMode) const
{
	switch (MovementMode)
	{
	case EMovementMode::MOVE_None:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.None"));
	case EMovementMode::MOVE_Walking:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Walking"));
	case EMovementMode::MOVE_NavWalking:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.NavWalking"));
	case EMovementMode::MOVE_Falling:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Falling"));
	case EMovementMode::MOVE_Swimming:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Swimming"));
	case EMovementMode::MOVE_Flying:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Flying"));
	case EMovementMode::MOVE_Custom:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.Custom"));
	default:
		return FGameplayTag::RequestGameplayTag(TEXT("State.MovementMode.None"));
	}
}

const UCharacterSettings* UCharacterSettings::Get()
{
	return GetDefault<UCharacterSettings>();
}


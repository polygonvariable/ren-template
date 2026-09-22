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
}

const UCharacterSettings* UCharacterSettings::Get()
{
	return GetDefault<UCharacterSettings>();
}


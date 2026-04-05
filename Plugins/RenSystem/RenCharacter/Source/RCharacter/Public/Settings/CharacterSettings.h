// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "CharacterSettings.generated.h"



/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig)
class UCharacterSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCharacterSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributeHealthTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributeMaxHealthTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributeLevelTag;


	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributePhysicalDamageTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributePhysicalDefenseTag;


	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributeElementalDamageTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag AttributeElementalDefenseTag;


	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag StateAliveTag;

	UPROPERTY(Config, EditDefaultsOnly)
	FGameplayTag StateDeadTag;


	static const UCharacterSettings* Get()
	{
		return GetDefault<UCharacterSettings>();
	}

};


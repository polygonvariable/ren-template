// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "AbilitySettings.generated.h"

// Forward Declarations
class UGameplayEffect;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Ability"))
class UAbilitySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UAbilitySettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DeadEffect;

	
	static const UAbilitySettings* Get()
	{
		return GetDefault<UAbilitySettings>();
	}

};


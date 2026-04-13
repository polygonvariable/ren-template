// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "AscensionSettings.generated.h"


/**
 *
 */
UCLASS(Config = RenProject, DefaultConfig, Meta = (DisplayName = "RCore - Ascension"))
class UAscensionSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UAscensionSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	int MinLevel = 1;

	UPROPERTY(Config, EditDefaultsOnly)
	int MaxLevel = 100;

	static const UAscensionSettings* Get()
	{
		return GetDefault<UAscensionSettings>();
	}

};


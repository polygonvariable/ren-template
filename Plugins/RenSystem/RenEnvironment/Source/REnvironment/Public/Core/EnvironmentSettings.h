// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "EnvironmentSettings.generated.h"

// Module Macros
#define REN_API RENVIRONMENT_API


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Environment"))
class UEnvironmentSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEnvironmentSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FName> EnvironmentBundles;

	REN_API static const UEnvironmentSettings* Get();

};


// Module Macros
#undef REN_API


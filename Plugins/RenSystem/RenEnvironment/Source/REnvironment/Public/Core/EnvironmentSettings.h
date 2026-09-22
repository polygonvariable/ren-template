// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "EnvironmentSettings.generated.h"

// Forward Declaration
class UEnvironmentSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Environment"))
class UEnvironmentSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEnvironmentSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UEnvironmentSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FName> EnvironmentBundles;

public:

	static RENVIRONMENT_API const UEnvironmentSettings* Get();

};


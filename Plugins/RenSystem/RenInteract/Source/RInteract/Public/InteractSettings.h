// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "InteractSettings.generated.h"

// Forward Declarations
class UInteractSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Interact"))
class UInteractSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UInteractSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<UInteractSubsystem> SubsystemClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Gameplay Mode")
	FName InteractMode;

public:

	static const UInteractSettings* Get();

};


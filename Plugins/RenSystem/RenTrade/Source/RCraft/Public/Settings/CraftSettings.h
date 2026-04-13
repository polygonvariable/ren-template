// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "CraftSettings.generated.h"

// Forward Declarations
class UCraftStorage;
class UCraftSubsystem;



/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Trade Craft"))
class UCraftSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCraftSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	FName StorageId;

	UPROPERTY(Config, EditDefaultsOnly)
	FString StorageUrl;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UCraftStorage> StorageClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<UCraftSubsystem> SubsystemClass;


	static const UCraftSettings* Get()
	{
		return GetDefault<UCraftSettings>();
	}

};


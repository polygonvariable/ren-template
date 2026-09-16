// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "StorageSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Storage"))
class UStorageSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UStorageSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	bool bEnableAutoSave = false;

	UPROPERTY(Config, EditDefaultsOnly)
	int AutoSaveInterval = 60;

	static const UStorageSettings* Get()
	{
		return GetDefault<UStorageSettings>();
	}

};


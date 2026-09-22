// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "CharacterSpawnerSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Character Spawner"))
class UCharacterSpawnerSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCharacterSpawnerSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> CharacterBundles;

public:

	static const UCharacterSpawnerSettings* Get();

};


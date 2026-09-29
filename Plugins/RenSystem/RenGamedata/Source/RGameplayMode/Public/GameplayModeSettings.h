// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "GameplayModeSettings.generated.h"

// Forward Declaration
class UDataTable;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Gameplay Mode"))
class UGameplayModeSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UGameplayModeSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Gameplay Mode", meta = (RequiredAssetDataTags = "RowStructure=/Script/RGameplayMode.GameplayModeTagGroup"))
	TSoftObjectPtr<UDataTable> GameplayModeTable;

public:

	static const UGameplayModeSettings* Get();

	UFUNCTION()
	static TArray<FName> GetGameplayModeTableRows();

};


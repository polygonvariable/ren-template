// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataAsset.h"

// Project Headers

// Generated Headers
#include "GameMetadataSettings.generated.h"



/**
 *
 */
UCLASS(Config = Game, defaultconfig, Meta = (DisplayName = "Game Metadata"))
class RCORESETTINGS_API UGameMetadataSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Inventory")
	TSoftObjectPtr<UDataTable> InventoryTable;

	UPROPERTY(Config, EditAnywhere, Meta = (AllowedClasses = "/Script/RenAsset.PrimaryAssetMap"), Category = "Inventory")
	TSoftObjectPtr<UPrimaryDataAsset> InventoryAssetMap;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataAsset.h"

// Project Headers

// Module Macros
#define REN_API RCORESETTINGS_API

// Generated Headers
#include "GameMetadataSettings.generated.h"



/**
 *
 */
UCLASS(Config = Game, defaultconfig, Meta = (DisplayName = "Game Metadata"))
class REN_API UGameMetadataSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FPrimaryAssetId> PreloadAssets;

};



// Module Macros
#undef REN_API


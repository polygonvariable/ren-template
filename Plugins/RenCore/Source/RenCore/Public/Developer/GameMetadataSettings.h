// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

// Project Headers

// Generated Headers
#include "GameMetadataSettings.generated.h"


/**
 *
 */
UCLASS(Config = Game, defaultconfig, Meta = (DisplayName = "Game Metadata"))
class RENCORE_API UGameMetadataSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TSoftObjectPtr<UDataTable> InventoryTable;

};


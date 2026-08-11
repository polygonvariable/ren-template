// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Project Headers
#include "Core/Type/Runtime/AvatarInstance.h"

// Generated Headers
#include "AvatarStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FAvatarInstance> AvatarCollection;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "EnemyStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UEnemyStorage : public USaveGame
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FGuid, FDateTime> KillTimestamps;

	UPROPERTY()
	TMap<FPrimaryAssetId, int> KillCounts;

};


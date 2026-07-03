// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/SaveGame.h"

// Generated Headers
#include "QuestStorage.generated.h"


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestStorage : public USaveGame
{

	GENERATED_BODY()

public:

	/* TMap<QuestId, Eventflow NodeId> */
	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FGuid> ActiveQuests;

};


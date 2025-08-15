// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

// Generated Headers
#include "EnhanceLibrary.generated.h"



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnhanceLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	RENCOREENHANCE_API static int CalculateLevelUpProgression(int Level, int Value, int ValueProgression);

    RENCOREENHANCE_API static int CalculateRequiredItemsForRankCap(int CurrentLevel, int CurrentXp, int Rank, int Points, int XpInterval, int LevelInterval);
	
	RENCOREENHANCE_API static bool CalculateLevelUp(
        int InPoints, int InXp, int InLevel, int InRank, int InXpInterval, int InLevelInterval, int InMaxLevel,
        int& OutXp, int& OutLevel, bool& bOutDoesLevelUpdated, bool& bOutDoesRankShouldUpdate, bool& bOutDoesMaxLevelReached);

	RENCOREENHANCE_API static bool CanRankUp(int Xp, int Level, int Rank, int LevelInterval);
	
};


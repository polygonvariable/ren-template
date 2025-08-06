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
UCLASS()
class RENCORE_API UEnhanceLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	static int CalculateLevelUpProgression(int Level, int Value, int ValueProgression)
    {
        return (Level - 1) * ValueProgression + Value;
    }

    static int CalculateRequiredItemsForRankCap(
        int CurrentLevel,
        int CurrentXp,
        int Rank,
        int Points,
        int XpInterval,
        int LevelInterval
    )
    {
        if (Points <= 0 || XpInterval <= 0 || LevelInterval <= 0)
        {
            return 0;
        }

        int MaxLevelForRank = Rank * LevelInterval;
        if (CurrentLevel >= MaxLevelForRank)
        {
            return 0;
        }

        int RemainingLevels = MaxLevelForRank - CurrentLevel;
        int TotalRequiredXp = RemainingLevels * XpInterval - CurrentXp;

        if (TotalRequiredXp <= 0)
        {
            return 0;
        }

        return (TotalRequiredXp + Points - 1) / Points;
    }
	
	static bool CalculateLevelUp(
        int InPoints, int InXp, int InLevel, int InRank, int InXpInterval, int InLevelInterval, int InMaxLevel,
        int& OutXp, int& OutLevel, bool& bOutDoesLevelUpdated, bool& bOutDoesRankShouldUpdate, bool& bOutDoesMaxLevelReached)
    {
        int LocalNewXp = InXp;
        int LocalNewLevel = InLevel;
        int LocalLevelCapForRank = InRank * InLevelInterval;
        int LocalLevelGained = 0;

        if (InLevel >= LocalLevelCapForRank)
        {
            bOutDoesRankShouldUpdate = true;
            return false;
        }

        LocalNewXp += InPoints;
        LocalLevelGained = LocalNewXp / InXpInterval;

        if (LocalLevelGained > 0)
        {
            LocalNewXp = 0;
            LocalNewLevel = LocalLevelGained + InLevel;
            bOutDoesLevelUpdated = true;

            if (LocalNewLevel >= LocalLevelCapForRank)
            {
                LocalNewXp = 0;
                LocalNewLevel = LocalLevelCapForRank;
                bOutDoesRankShouldUpdate = true;
            }
            if (LocalNewLevel > InMaxLevel)
            {
                LocalNewXp = 0;
                LocalNewLevel = InMaxLevel;
                bOutDoesMaxLevelReached = true;
            }
        }

        OutXp = LocalNewXp;
        OutLevel = LocalNewLevel;

        return true;
    }

	static bool CanRankUp(int Xp, int Level, int LevelInterval)
    {
        return Xp == 0 && Level % LevelInterval == 0;
    }
	
};


// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/EnhanceLibrary.h"

int UEnhanceLibrary::CalculateLevelUpProgression(int Level, int Value, int ValueProgression)
{
    return (Level - 1) * ValueProgression + Value;
}

bool UEnhanceLibrary::CalculateEnhance(
    int EnhancePoint,
    int CurrentXp,
    int CurrentLevel,
    int CurrentRank,
    int XpInterval,
    int LevelInterval,
    int MaxLevel,
    int& NewXp,
    int& NewLevel,
    bool& bDoesLevelUpdated,
    bool& bDoesRankShouldUpdate,
    bool& bDoesMaxLevelReached
)
{

    int LocalNewXp = CurrentXp;
    int LocalNewLevel = CurrentLevel;
    int LocalLevelCapForRank = CurrentRank * LevelInterval;
    int LocalLevelGained = 0;

    if (CurrentLevel >= LocalLevelCapForRank)
    {
        bDoesRankShouldUpdate = true;
        return false;
    }

    LocalNewXp += EnhancePoint;
    LocalLevelGained = LocalNewXp / XpInterval;

    if (LocalLevelGained > 0)
    {
        LocalNewXp = 0;
        LocalNewLevel = LocalLevelGained + CurrentLevel;
        bDoesLevelUpdated = true;

        if (LocalNewLevel >= LocalLevelCapForRank)
        {
            LocalNewXp = 0;
            LocalNewLevel = LocalLevelCapForRank;
            bDoesRankShouldUpdate = true;
        }
        if (LocalNewLevel > MaxLevel)
        {
            LocalNewXp = 0;
            LocalNewLevel = MaxLevel;
            bDoesMaxLevelReached = true;
        }
    }

    NewXp = LocalNewXp;
	NewLevel = LocalNewLevel;

    return true;
}

bool UEnhanceLibrary::CanRankUp(const FEnhanceRecord& EnhanceRecord, const int LevelInterval)
{
    return EnhanceRecord.Experience == 0 && EnhanceRecord.Level % LevelInterval == 0;
}

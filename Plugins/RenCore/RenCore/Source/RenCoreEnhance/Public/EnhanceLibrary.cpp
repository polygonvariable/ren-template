// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceLibrary.h"

// Engine Headers

// Project Headers



int UEnhanceLibrary::CalculateLevelUpProgression(int Level, int Value, int ValueProgression)
{
    return (Level - 1) * ValueProgression + Value;
}

int UEnhanceLibrary::CalculateRequiredItemsForRankCap(int CurrentLevel, int CurrentXp, int Rank, int Points, int XpInterval, int LevelInterval)
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

bool UEnhanceLibrary::CalculateLevelUp(
        int InPoints, int InXp, int InLevel, int InRank, int InXpInterval, int InLevelInterval, int InMaxLevel,
        int& OutXp, int& OutLevel, bool& bOutDoesLevelUpdated, bool& bOutDoesRankShouldUpdate, bool& bOutDoesMaxLevelReached)
{
    int LocalNewXp = InXp + InPoints;
    int LocalNewLevel = InLevel;
    bOutDoesLevelUpdated = false;
    bOutDoesRankShouldUpdate = false;
    bOutDoesMaxLevelReached = false;

    const int LocalLevelCapForRank = InRank * InLevelInterval;

    if (InLevel >= LocalLevelCapForRank)
    {
        OutXp = 0;
        OutLevel = InLevel;
        bOutDoesRankShouldUpdate = true;
        return false;
    }

    int MaxGainableLevels = LocalLevelCapForRank - InLevel;
    int GainedLevels = LocalNewXp / InXpInterval;

    if (GainedLevels > 0)
    {
        int AppliedLevels = FMath::Min(GainedLevels, MaxGainableLevels);
        LocalNewLevel += AppliedLevels;
        LocalNewXp -= AppliedLevels * InXpInterval;
        bOutDoesLevelUpdated = true;

        if (LocalNewLevel >= LocalLevelCapForRank)
        {
            LocalNewLevel = LocalLevelCapForRank;
            LocalNewXp = 0;
            bOutDoesRankShouldUpdate = true;
        }
    }

    if (LocalNewLevel >= InMaxLevel)
    {
        LocalNewLevel = InMaxLevel;
        LocalNewXp = 0;
        bOutDoesMaxLevelReached = true;
    }

    OutXp = LocalNewXp;
    OutLevel = LocalNewLevel;
    return true;
}

bool UEnhanceLibrary::CanRankUp(int Xp, int Level, int Rank, int LevelInterval)
{
    return Xp == 0 && Level % LevelInterval == 0 && Level / LevelInterval == Rank;
}


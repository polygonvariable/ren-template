// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceLibrary.h"

// Engine Headers

// Project Headers



int UEnhanceLibrary::CalculateLevelUpProgression(int Level, int Value, int ValueProgression)
{
    return (Level - 1) * ValueProgression + Value;
}

int UEnhanceLibrary::CalculateRequiredItemsForRankCap(int CurrentLevel, int CurrentExperience, int Rank, int Points, int ExperienceInterval, int LevelInterval)
{
    if (Points <= 0 || ExperienceInterval <= 0 || LevelInterval <= 0)
    {
        return 0;
    }

    int MaxLevelForRank = Rank * LevelInterval;
    if (CurrentLevel >= MaxLevelForRank)
    {
        return 0;
    }

    int RemainingLevels = MaxLevelForRank - CurrentLevel;
    int TotalRequiredExperience = RemainingLevels * ExperienceInterval - CurrentExperience;

    if (TotalRequiredExperience <= 0)
    {
        return 0;
    }

    return (TotalRequiredExperience + Points - 1) / Points;
}

bool UEnhanceLibrary::CalculateLevelUp(
    int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel,
    int& OutExperience, int& OutLevel, bool& bOutDoesLevelUpdated, bool& bOutDoesRankShouldUpdate, bool& bOutDoesMaxLevelReached)
{
    int LocalNewExperience = InExperience + InPoints;
    int LocalNewLevel = InLevel;
    bOutDoesLevelUpdated = false;
    bOutDoesRankShouldUpdate = false;
    bOutDoesMaxLevelReached = false;

    const int LocalLevelCapForRank = InRank * InLevelInterval;

    if (InLevel >= LocalLevelCapForRank)
    {
        OutExperience = 0;
        OutLevel = InLevel;
        bOutDoesRankShouldUpdate = true;
        return false;
    }

    int MaxGainableLevels = LocalLevelCapForRank - InLevel;
    int GainedLevels = LocalNewExperience / InExperienceInterval;

    if (GainedLevels > 0)
    {
        int AppliedLevels = FMath::Min(GainedLevels, MaxGainableLevels);
        LocalNewLevel += AppliedLevels;
        LocalNewExperience -= AppliedLevels * InExperienceInterval;
        bOutDoesLevelUpdated = true;

        if (LocalNewLevel >= LocalLevelCapForRank)
        {
            LocalNewLevel = LocalLevelCapForRank;
            LocalNewExperience = 0;
            bOutDoesRankShouldUpdate = true;
        }
    }

    if (LocalNewLevel >= InMaxLevel)
    {
        LocalNewLevel = InMaxLevel;
        LocalNewExperience = 0;
        bOutDoesMaxLevelReached = true;
    }

    OutExperience = LocalNewExperience;
    OutLevel = LocalNewLevel;
    return true;
}

bool UEnhanceLibrary::CalculateLevelUp(int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel, int& OutExperience, int& OutLevel)
{
    bool bDoesLevelUpdated = false;
    bool bDoesRankShouldUpdate = false;
    bool bDoesMaxLevelReached = false;

    return CalculateLevelUp(
        InPoints, InExperience, InLevel, InRank, InExperienceInterval, InLevelInterval, InMaxLevel, OutExperience, OutLevel,
        bDoesLevelUpdated, bDoesRankShouldUpdate, bDoesMaxLevelReached
    );
}

bool UEnhanceLibrary::CanLevelUp(int Experience, int Level, int Rank, int ExperienceInterval, int LevelInterval, int MaxLevel)
{
    if (ExperienceInterval <= 0 || LevelInterval <= 0)
    {
        return false;
    }

    const int LocalLevelCapForRank = Rank * LevelInterval;
    if (Level >= LocalLevelCapForRank)
    {
        return false;
    }

    if (Level >= MaxLevel)
    {
        return false;
    }

    return Experience >= ExperienceInterval;
}

bool UEnhanceLibrary::CanRankUp(int Experience, int Level, int Rank, int LevelInterval, int MaxRank)
{
    return Experience == 0 && Level % LevelInterval == 0 && Level / LevelInterval == Rank && Rank < MaxRank;
}


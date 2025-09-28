// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceLibrary.h"

// Engine Headers

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"



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

bool UEnhanceLibrary::CalculateLevelUp(int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel, int& OutExperience, int& OutLevel, bool& bOutLevelUpdated, bool& bOutRankShouldUpdate, bool& bOutMaxLevelReached)
{
    int LocalNewExperience = InExperience + InPoints;
    int LocalNewLevel = InLevel;
    bOutLevelUpdated = false;
    bOutRankShouldUpdate = false;
    bOutMaxLevelReached = false;

    const int LocalLevelCapForRank = InRank * InLevelInterval;

    if (InLevel >= LocalLevelCapForRank)
    {
        OutExperience = 0;
        OutLevel = InLevel;
        bOutRankShouldUpdate = true;
        return false;
    }

    int MaxGainableLevels = LocalLevelCapForRank - InLevel;
    int GainedLevels = LocalNewExperience / InExperienceInterval;

    if (GainedLevels > 0)
    {
        int AppliedLevels = FMath::Min(GainedLevels, MaxGainableLevels);
        LocalNewLevel += AppliedLevels;
        LocalNewExperience -= AppliedLevels * InExperienceInterval;
        bOutLevelUpdated = true;

        if (LocalNewLevel >= LocalLevelCapForRank)
        {
            LocalNewLevel = LocalLevelCapForRank;
            LocalNewExperience = 0;
            bOutRankShouldUpdate = true;
        }
    }

    if (LocalNewLevel >= InMaxLevel)
    {
        LocalNewLevel = InMaxLevel;
        LocalNewExperience = 0;
        bOutMaxLevelReached = true;
    }

    OutExperience = LocalNewExperience;
    OutLevel = LocalNewLevel;
    return true;
}

bool UEnhanceLibrary::CalculateLevelUp(int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel, int& OutExperience, int& OutLevel)
{
    bool bLevelUpdated = false;
    bool bRankShouldUpdate = false;
    bool bMaxLevelReached = false;

    return CalculateLevelUp(
        InPoints, InExperience, InLevel, InRank, InExperienceInterval, InLevelInterval, InMaxLevel,
        OutExperience, OutLevel, bLevelUpdated, bRankShouldUpdate, bMaxLevelReached
    );
}

bool UEnhanceLibrary::CalculateLevelUp(int InPoints, const FEnhanceRecord& InEnhanceRecord, int InExperienceInterval, int InLevelInterval, int InMaxLevel, int& OutExperience, int& OutLevel)
{
    return CalculateLevelUp(
        InPoints,
        InEnhanceRecord.Experience,
        InEnhanceRecord.Level,
        InEnhanceRecord.Rank,
        InExperienceInterval,
        InLevelInterval,
        InMaxLevel,
        OutExperience,
        OutLevel
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

bool UEnhanceLibrary::CanLevelUp(const FEnhanceRecord& EnhanceRecord, int ExperienceInterval, int LevelInterval, int MaxLevel)
{
    return CanLevelUp(
        EnhanceRecord.Experience,
        EnhanceRecord.Level,
        EnhanceRecord.Rank,
        ExperienceInterval,
        LevelInterval,
        MaxLevel
    );
}

bool UEnhanceLibrary::CanRankUp(int Experience, int Level, int Rank, int LevelInterval, int MaxRank)
{
    return Experience == 0 && Level % LevelInterval == 0 && Level / LevelInterval == Rank && Rank < MaxRank;
}

bool UEnhanceLibrary::CanRankUp(const FEnhanceRecord& EnhanceRecord, int LevelInterval, int MaxRank)
{
    return CanRankUp(
        EnhanceRecord.Experience,
        EnhanceRecord.Level,
        EnhanceRecord.Rank,
        LevelInterval,
        MaxRank
    );
}


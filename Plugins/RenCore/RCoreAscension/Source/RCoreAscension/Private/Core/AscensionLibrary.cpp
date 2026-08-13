// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/AscensionLibrary.h"

// Project Headers
#include "Core/Type/AscensionData.h"
#include "Core/AscensionSettings.h"


int FAscensionLibrary::ClampLevel(int Level)
{
    const UAscensionSettings* Settings = UAscensionSettings::Get();

    return FMath::Clamp(Level, Settings->MinLevel, Settings->MaxLevel);
}

int FAscensionLibrary::ScaleByLevel(int Value, int Level)
{
    return FMath::Clamp(Value * Level, 0, INT32_MAX); // Linear
}

bool FAscensionLibrary::AddExperience(const FAscensionData& CurrentData, int Amount, int ExperiencePerLevel, int LevelPerRank, int MaxLevel, int MaxRank, int& OutExperience, int& OutLevel)
{
    OutExperience = CurrentData.Experience;
	OutLevel = CurrentData.Level;

    if (Amount <= 0 || ExperiencePerLevel <= 0)
    {
        return false;
    }

    if (!CanGainExperience(CurrentData, LevelPerRank, MaxLevel, MaxRank))
    {
        return false;
    }

    OutExperience += Amount;

    const int MaxLevelForRank = GetMaxLevelForRank(CurrentData.Rank, LevelPerRank, MaxLevel);

    while (OutExperience >= ExperiencePerLevel)
    {
        if (OutLevel >= MaxLevelForRank)
        {
            break;
        }

        OutExperience -= ExperiencePerLevel;
        OutLevel++;

        if (OutLevel >= MaxLevel && CurrentData.Rank >= MaxRank)
        {
            OutLevel = MaxLevel;
            OutExperience = 0;
            break;
        }
    }

    return true;
}

bool FAscensionLibrary::CanGainExperience(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank)
{
    if (Data.Rank > MaxRank)
    {
        return false;
    }

    if (Data.Rank == MaxRank && Data.Level >= MaxLevel)
    {
        return false;
    }

    const int MaxLevelForRank = GetMaxLevelForRank(Data.Rank, LevelPerRank, MaxLevel);

    return Data.Level < MaxLevelForRank;
}

bool FAscensionLibrary::IsRankUpRequired(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank)
{
    if (Data.Rank >= MaxRank)
    {
        return false;
    }

    const int MaxLevelForRank = GetMaxLevelForRank(Data.Rank, LevelPerRank, MaxLevel);

    return Data.Level >= MaxLevelForRank;
}

int FAscensionLibrary::GetMaxLevelForRank(int Rank, int LevelPerRank, int MaxLevel)
{
    const int32 CalculatedMax = Rank * LevelPerRank;
    return FMath::Min(CalculatedMax, MaxLevel);
}

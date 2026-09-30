// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declarations
struct FAscensionData;


/**
 *
 */
class FAscensionLibrary
{

public:

    CGAMEDATAASCENSION_API static int ClampLevel(int Level);

    CGAMEDATAASCENSION_API static int ScaleByLevel(int Value, int Level);

    CGAMEDATAASCENSION_API static bool AddExperience(const FAscensionData& CurrentData, int Amount, int ExperiencePerLevel, int LevelPerRank, int MaxLevel, int MaxRank, int& OutExperience, int& OutLevel);
    CGAMEDATAASCENSION_API static bool CanGainExperience(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);
    CGAMEDATAASCENSION_API static bool IsRankUpRequired(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);

private:

    static int GetMaxLevelForRank(int Rank, int LevelPerRank, int MaxLevel);

};


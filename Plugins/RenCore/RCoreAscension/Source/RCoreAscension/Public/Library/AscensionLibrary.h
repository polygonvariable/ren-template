// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCOREASCENSION_API

// Forward Declarations
struct FAscensionData;



/**
 *
 */
class FAscensionLibrary
{

public:

    REN_API static int ScaleByLevel(int Value, int Level);

    REN_API static bool AddExperience(const FAscensionData& CurrentData, int Amount, int ExperiencePerLevel, int LevelPerRank, int MaxLevel, int MaxRank, int& OutExperience, int& OutLevel);
    REN_API static bool CanGainExperience(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);
    REN_API static bool IsRankUpRequired(const FAscensionData& Data, int LevelPerRank, int MaxLevel, int MaxRank);

private:

    static int GetMaxLevelForRank(int Rank, int LevelPerRank, int MaxLevel);

};



// Module Macros
#undef REN_API


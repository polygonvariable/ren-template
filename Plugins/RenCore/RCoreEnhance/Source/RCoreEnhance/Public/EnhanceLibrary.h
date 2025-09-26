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

	RCOREENHANCE_API static int CalculateLevelUpProgression(int Level, int Value, int ValueProgression);

    RCOREENHANCE_API static int CalculateRequiredItemsForRankCap(int CurrentLevel, int CurrentExperience, int Rank, int Points, int ExperienceInterval, int LevelInterval);
	
	RCOREENHANCE_API static bool CalculateLevelUp(
        int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel,
        int& OutExperience, int& OutLevel, bool& bOutDoesLevelUpdated, bool& bOutDoesRankShouldUpdate, bool& bOutDoesMaxLevelReached
	);

	RCOREENHANCE_API static bool CalculateLevelUp(
		int InPoints, int InExperience, int InLevel, int InRank, int InExperienceInterval, int InLevelInterval, int InMaxLevel,
		int& OutExperience, int& OutLevel
	);

	RCOREENHANCE_API static bool CanLevelUp(int Experience, int Level, int Rank, int ExperienceInterval, int LevelInterval, int MaxLevel);
	RCOREENHANCE_API static bool CanRankUp(int Experience, int Level, int Rank, int LevelInterval, int MaxRank);
	
};


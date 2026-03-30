// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AscensionData.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FAscensionData
{

	GENERATED_BODY()

public:

	FAscensionData() {}
	FAscensionData(int InExperience, int InLevel, int InRank) : Experience(InExperience), Level(InLevel), Rank(InRank) {}

	UPROPERTY(EditAnywhere, SaveGame)
	int Experience = 0;

	UPROPERTY(EditAnywhere, SaveGame)
	int Level = 1;

	UPROPERTY(EditAnywhere, SaveGame)
	int Rank = 1;

	bool IsValid() const
	{
		return Level > 0 && Rank > 0;
	}

	void Reset()
	{
		Experience = 0;
		Level = 1;
		Rank = 1;
	}

	void Sanitize()
	{
		Rank = FMath::Max(1, Rank);
		Level = FMath::Max(1, Level);
		Experience = FMath::Max(0, Experience);
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("Experience: %d, Level: %d, Rank: %d"), Experience, Level, Rank);
	}

};


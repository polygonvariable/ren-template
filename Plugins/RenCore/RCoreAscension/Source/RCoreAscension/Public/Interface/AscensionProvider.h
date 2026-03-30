// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AscensionProvider.generated.h"

// Module Macros
#define REN_API RCOREASCENSION_API

// Forward Declarations
class UAssetCollection;
struct FAscensionData;


UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UAscensionProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class REN_API IAscensionProvider
{

	GENERATED_BODY()

public:

	virtual int GetExperienceInterval(int Level) const = 0;
	virtual int GetLevelInterval(int Rank) const = 0;
	virtual int GetMaxLevel() const = 0;
	virtual int GetMaxRank() const = 0;

	virtual const UAssetCollection* GetExperienceAssets(const FAscensionData& Ascension) const = 0;
	virtual const UAssetCollection* GetRankAssets(const FAscensionData& Ascension) const = 0;

};

// Module Macros
#undef REN_API


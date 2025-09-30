// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "EnhanceProviderInterface.generated.h"

// Forward Declarations
struct FExchangeRule;



UINTERFACE(MinimalAPI)
class UEnhanceProviderInterface : public UInterface
{
	
	GENERATED_BODY()

};

/**
 * 
 */
class RCOREENHANCE_API IEnhanceProviderInterface
{

	GENERATED_BODY()

public:

	virtual int GetExperienceInterval(int Level) const = 0;
	virtual int GetLevelInterval(int Rank) const = 0;
	virtual int GetMaxLevel() const = 0;
	virtual int GetMaxRank() const = 0;
	virtual const FExchangeRule& GetEnhanceRules() const = 0;
	virtual const TArray<FExchangeRule>& GetRankingRules() const = 0;

};


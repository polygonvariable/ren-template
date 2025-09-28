// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreFilter/Public/FilterContext.h"
#include "RCoreFilter/Public/FilterCriterion.h"

// Generated Headers
#include "FilterParentCriterion.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (AND/OR)")
class UFilterGroupCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	EFilterCombination Combination = EFilterCombination::And;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UFilterCriterion>> Criteria;

	RCOREFILTER_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (NOT)")
class UFilterNotCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterCriterion> Negate;

	RCOREFILTER_API virtual bool Evaluate(const FFilterContext& Context) const override;

};


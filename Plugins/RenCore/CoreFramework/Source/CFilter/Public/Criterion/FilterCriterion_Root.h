// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/QueryType.h"
#include "FilterContext.h"
#include "FilterCriterion.h"

// Generated Headers
#include "FilterCriterion_Root.generated.h"

// Forward Declaration
struct FFilterContext;


/**
 *
 */
USTRUCT()
struct FFilterCriterion_Group : public FFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	EFilterOperator Operator = EFilterOperator::And;

	UPROPERTY(EditAnywhere)
	TArray<TInstancedStruct<FFilterCriterion>> Criteria;

	// ~ UFilterCriterion
	virtual bool Evaluate(const FFilterContext& Context) const override;
	// ~ End of UFilterCriterion

};


/**
 *
 */
USTRUCT()
struct FFilterCriterion_Not : public FFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TInstancedStruct<FFilterCriterion> Negate;

	// ~ UFilterCriterion
	virtual bool Evaluate(const FFilterContext& Context) const override;
	// ~ End of UFilterCriterion
};



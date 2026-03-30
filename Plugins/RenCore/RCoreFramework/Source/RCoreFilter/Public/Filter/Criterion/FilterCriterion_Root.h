// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/QueryType.h"
#include "Filter/FilterCriterion.h"

// Generated Headers
#include "FilterCriterion_Root.generated.h"

// Module Macros
#define REN_API RCOREFILTER_API

// Forward Declarations
struct FFilterContext;



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (AND/OR)")
class UFilterCriterion_Group : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFilterOperator Operator = EFilterOperator::And;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TArray<TObjectPtr<UFilterCriterion>> Criteria;


	// ~ UFilterCriterion
	virtual bool Evaluate(const FFilterContext& Context) const override;
	// ~ End of UFilterCriterion

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (NOT)")
class UFilterCriterion_Not : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterCriterion> Negate;


	// ~ UFilterCriterion
	virtual bool Evaluate(const FFilterContext& Context) const override;
	// ~ End of UFilterCriterion

};


// Module Macros
#undef REN_API


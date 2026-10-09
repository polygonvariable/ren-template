// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "FilterContext.h"

// Generated Headers
#include "FilterCriterion.generated.h"

// Forward Declarations
struct FFilterContext;


/**
 *
 */
USTRUCT()
struct FFilterCriterion
{

	GENERATED_BODY()

public:

	virtual bool Evaluate(const FFilterContext& Context) const
	{
		return false;
	}

	virtual FName GetPropertyName() const
	{
		return NAME_None;
	}
	virtual bool GetIsLeaf() const
	{
		return false;
	}

	virtual void ClearEvaluationData() {};
	virtual void CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other) {};

	virtual ~FFilterCriterion() = default;

};


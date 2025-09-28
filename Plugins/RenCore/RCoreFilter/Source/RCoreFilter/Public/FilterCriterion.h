// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "FilterCriterion.generated.h"

// Forward Declarations
struct FFilterContext;



/**
 *
 */
UCLASS(MinimalAPI, Abstract, Blueprintable, EditInlineNew, CollapseCategories)
class UFilterCriterion : public UObject
{

	GENERATED_BODY()

public:

	virtual bool Evaluate(const FFilterContext& Context) const;
	virtual FName GetPropertyName() const;
	virtual bool GetIsLeaf() const;

};


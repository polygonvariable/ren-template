// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "FilterGroup.generated.h"

// Module Macros
#define REN_API RCOREFILTER_API

// Forward Declarations
class UFilterCriterion;



/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UFilterGroup : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UFilterCriterion> CriterionRoot;


	REN_API UFilterCriterion* GetCriterionByName(FName PropertyName) const;

	template<typename T>
	T* GetCriterionByName(FName PropertyName) const
	{
		return Cast<T>(GetCriterionByName(PropertyName));
	}

protected:

	UFilterCriterion* FindCriterionByName(UFilterCriterion* Criterion, const FName& PropertyName) const;

};



// Module Macros
#undef REN_API


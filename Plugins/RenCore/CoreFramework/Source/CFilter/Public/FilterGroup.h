// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "FilterCriterion.h"

// Generated Headers
#include "FilterGroup.generated.h"


/**
 *
 */
USTRUCT()
struct FFilterGroup
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TInstancedStruct<FFilterCriterion> CriterionRoot;

	CFILTER_API TInstancedStruct<FFilterCriterion>* GetCriterionByName(FName PropertyName);

protected:

	CFILTER_API TInstancedStruct<FFilterCriterion>* FindCriterionByName(TInstancedStruct<FFilterCriterion>& Criterion, const FName& PropertyName);

};


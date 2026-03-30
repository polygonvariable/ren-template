// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Filter/FilterCriterion.h"



bool UFilterCriterion::Evaluate(const FFilterContext& Context) const
{
	return true;
}

FName UFilterCriterion::GetPropertyName() const
{
	return NAME_None;
}

bool UFilterCriterion::GetIsLeaf() const
{
	return false;
}

void UFilterCriterion::ClearEvaluationData()
{
}

void UFilterCriterion::CopyEvaluationData(const UFilterCriterion* Other)
{
}


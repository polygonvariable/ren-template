// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreFilter/Public/FilterCriterion.h"

// Generated Headers
#include "FilterLeafCriterion.generated.h"

// Forward Declarations



/**
 *
 */
UCLASS(Abstract)
class UFilterLeafCriterion : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName PropertyName;

	virtual FName GetPropertyName() const override;
	virtual bool GetIsLeaf() const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Text)")
class UFilterTextCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FName> Included;

	RCOREFILTER_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Asset)")
class UFilterAssetCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FPrimaryAssetId> Included;

	RCOREFILTER_API virtual bool Evaluate(const FFilterContext& Context) const override;

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Integer)")
class UFilterIntegerCriterion : public UFilterLeafCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int Min = 0;

	UPROPERTY(EditAnywhere)
	int Max = 0;

	UPROPERTY(EditAnywhere)
	bool bEnableStrictMode = true;

	RCOREFILTER_API virtual bool Evaluate(const FFilterContext& Context) const override;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Filter/FilterCriterion.h"

// Generated Headers
#include "FilterCriterion_Leaf.generated.h"

// Module Macros
#define REN_API RCOREFILTER_API



/**
 *
 */
UCLASS(Abstract)
class UFilterCriterion_Leaf : public UFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PropertyName;

	// ~ UFilterCriterion
	virtual FName GetPropertyName() const override;
	virtual bool GetIsLeaf() const override;
	// ~ End of UFilterCriterion

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Text)")
class UFilterCriterion_Text : public UFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Included;


	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const UFilterCriterion* Other) override;
	// ~ End of UFilterCriterion_Leaf

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Guid)")
class UFilterCriterion_Guid : public UFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGuid> Included;


	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const UFilterCriterion* Other) override;
	// ~ End of UFilterCriterion_Leaf

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Asset)")
class UFilterCriterion_Asset : public UFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPrimaryAssetId> Included;


	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const UFilterCriterion* Other) override;
	// ~ End of UFilterCriterion_Leaf

};



/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Filter (Integer)")
class UFilterCriterion_Integer : public UFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Min = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Max = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableStrictMode = true;


	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const UFilterCriterion* Other) override;
	// ~ End of UFilterCriterion_Leaf

};


// Module Macros
#undef REN_API


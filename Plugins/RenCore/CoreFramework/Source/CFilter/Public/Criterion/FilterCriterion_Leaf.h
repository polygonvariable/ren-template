// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "FilterContext.h"
#include "FilterCriterion.h"

// Generated Headers
#include "FilterCriterion_Leaf.generated.h"

// Forward Declarations
struct FFilterContext;


/**
 *
 */
USTRUCT()
struct CFILTER_API FFilterCriterion_Leaf : public FFilterCriterion
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName PropertyName;

	// ~ UFilterCriterion
	virtual FName GetPropertyName() const override;
	virtual bool GetIsLeaf() const override;
	// ~ End of UFilterCriterion

};


/**
 *
 */
USTRUCT(DisplayName = "Filter (Text)")
struct CFILTER_API FFilterCriterion_Text : public FFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FName> Included;

	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other) override;
	// ~ End of UFilterCriterion_Leaf

};


/**
 *
 */
USTRUCT(DisplayName = "Filter (Guid)")
struct CFILTER_API FFilterCriterion_Guid : public FFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FGuid> Included;

	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other) override;
	// ~ End of UFilterCriterion_Leaf

};


/**
 *
 */
USTRUCT(DisplayName = "Filter (Asset)")
struct CFILTER_API FFilterCriterion_Asset : public FFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FPrimaryAssetId> Included;

	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other) override;
	// ~ End of UFilterCriterion_Leaf

};


/**
 *
 */
USTRUCT(DisplayName = "Filter (Integer)")
struct CFILTER_API FFilterCriterion_Integer : public FFilterCriterion_Leaf
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	int Min = 0;

	UPROPERTY(EditAnywhere)
	int Max = 0;

	UPROPERTY(EditAnywhere)
	bool bEnableStrictMode = true;

	// ~ UFilterCriterion_Leaf
	virtual bool Evaluate(const FFilterContext& Context) const override;
	virtual void ClearEvaluationData() override;
	virtual void CopyEvaluationData(const TInstancedStruct<FFilterCriterion>& Other) override;
	// ~ End of UFilterCriterion_Leaf

};


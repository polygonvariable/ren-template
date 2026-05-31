// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "EventflowDefinition.h"

// Generated Headers
#include "EventflowAsset.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

/**
 *
 *
 */
UCLASS(MinimalAPI, BlueprintType)
class UEventflowAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FEventflowNodeDefinition> NodeCollection;

	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FEventflowPinRelation> PinRelation;

	UPROPERTY(VisibleAnywhere)
	FGuid EntryNodeId = FGuid::NewGuid();

	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

#if WITH_EDITOR

	// ~ UObject
	REN_API virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject

protected:
	
	// ~ UObject
	REN_API virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	// ~ End of UObject

#endif

public:

	static REN_API FPrimaryAssetType GetPrimaryAssetType();

};


// Module Macros
#undef REN_API


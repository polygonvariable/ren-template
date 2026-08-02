// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/ObjectSaveContext.h"

// Project Headers
#include "Type/EventflowGraphData.h"

// Generated Headers
#include "EventflowAsset.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API


/**
 *
 */
UCLASS(MinimalAPI, BlueprintType)
class UEventflowAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	/** TMap<NodeId, NodeDefinition> */
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FEventflowNode> NodeCollection;

	/** TMap<PinId(OutputPin), PinRelation(InputPin, NodeId)> */
	UPROPERTY(VisibleAnywhere)
	TMap<FGuid, FEventflowPinRelation> PinRelation;

	UPROPERTY(VisibleAnywhere)
	FGuid EntryNodeId = FGuid::NewGuid();


	// ~ UPrimaryDataAsset
	REN_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

#if WITH_EDITORONLY_DATA

	// ~ UPrimaryDataAsset
	REN_API virtual void Serialize(FArchive& Ar) override;
	REN_API virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	REN_API virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset

#endif

protected:

#if WITH_EDITOR

	// ~ UObject
	REN_API virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	// ~ End of UObject

#endif

public:

	static REN_API FPrimaryAssetType GetPrimaryAssetType();

};


// Module Macros
#undef REN_API


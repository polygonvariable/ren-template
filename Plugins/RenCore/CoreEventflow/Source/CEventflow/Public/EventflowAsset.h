// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Type/EventflowGraphData.h"

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declaration
class FObjectPreSaveRootContext;


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
	CEVENTFLOW_API virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~ End of UPrimaryDataAsset

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	CEVENTFLOW_API virtual void Serialize(FArchive& Ar) override;
	CEVENTFLOW_API virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	CEVENTFLOW_API virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset
#endif

protected:

#if WITH_EDITOR
	// ~ UObject
	CEVENTFLOW_API virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	// ~ End of UObject
#endif

public:

	static CEVENTFLOW_API FPrimaryAssetType GetPrimaryAssetType();

};


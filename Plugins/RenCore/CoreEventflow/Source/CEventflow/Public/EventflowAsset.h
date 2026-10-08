// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Type/EventflowGraphData.h"

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declaration
class FObjectPreSaveRootContext;
class UEventflowGlobalTask;


/**
 *
 */
UCLASS()
class CEVENTFLOW_API UEventflowAsset : public UPrimaryDataAsset
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


	virtual UEventflowGlobalTask* GetGlobalTask(FName TaskName) const;

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset
#endif

protected:

#if WITH_EDITOR
	// ~ UObject
	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	// ~ End of UObject
#endif

};


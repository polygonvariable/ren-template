// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declarations
class UEventflowData;



UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowAsset : public UObject
{

	GENERATED_BODY()

public:

	// UPROPERTY(EditAnywhere, Instanced)
	// TObjectPtr<UObject> SharedData;

	UPROPERTY()
	TObjectPtr<UEventflowData> GraphData = nullptr;

	DECLARE_MULTICAST_DELEGATE(FOnEventflowAssetSaved);
	FOnEventflowAssetSaved OnEventflowAssetSaved;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};


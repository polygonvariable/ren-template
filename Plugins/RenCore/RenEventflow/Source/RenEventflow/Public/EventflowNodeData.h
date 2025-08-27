// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeData.generated.h"

// Forward Declarations
class UEventflowNodeExternalData;
class UEventflowNodeDataController;



UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowNodeData : public UObject
{

	GENERATED_BODY()

public:

	// UPROPERTY(EditAnywhere, Instanced)
	// TObjectPtr<UEventflowNodeExternalData> ExternalData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEventflowNodeDataController> DataController;

	virtual const TArray<FText>* GetInputOptions() const { return nullptr; };
	virtual const TArray<FText>* GetOutputOptions() const { return nullptr; };

};


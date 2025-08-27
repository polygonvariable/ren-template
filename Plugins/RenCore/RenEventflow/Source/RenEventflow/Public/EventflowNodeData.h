// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeData.generated.h"

// Forward Declarations



UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowNodeData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TArray<FText> InputOptions;

	UPROPERTY(EditAnywhere)
	TArray<FText> OutputOptions;
	
	// UPROPERTY(EditAnywhere, Instanced)
	// TArray<TObjectPtr<UEventflowNodeExternalData>> ExternalData;

	const TArray<TPair<FName, FText>>* GetInputOptions() const { return nullptr; };
	const TArray<TPair<FName, FText>>* GetOutputOptions() const { return nullptr; };

};


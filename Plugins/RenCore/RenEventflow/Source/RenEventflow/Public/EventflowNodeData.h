// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeData.generated.h"

// Forward Declarations
class UEventflowNodeExternalData;
class UEventflowNodeTask;
class UEventflowBlueprint;



UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowNodeData : public UObject
{

	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, NonTransactional, Transient)
	TSubclassOf<UEventflowBlueprint> GraphBlueprint;

#endif

#if WITH_EDITOR

	UFUNCTION()
	TArray<FString> GetBlueprintMethods();

#endif

	// UPROPERTY(EditAnywhere, Instanced)
	// TObjectPtr<UEventflowNodeExternalData> ExternalData;

	UPROPERTY(EditAnywhere, NonTransactional, Meta = (GetOptions = "GetBlueprintMethods"))
	FName NodeEvent;

	// UPROPERTY(EditAnywhere)
	// TSubclassOf<UEventflowNodeTask> NodeTask;
	
	virtual const TArray<FText>* GetInputOptions() const { return nullptr; };
	virtual const TArray<FText>* GetOutputOptions() const { return nullptr; };

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeExternalData.generated.h"

// Forward Declarations



UCLASS(Abstract, Blueprintable, Meta = (ShowWorldContextPin))
class RENEVENTFLOW_API UEventflowNodeDataController : public UObject
{

	GENERATED_BODY()

public:

	bool GetWaitForCompletion() const;

	void StartTask();

	UFUNCTION(BlueprintCallable)
	void StopTask();

	DECLARE_MULTICAST_DELEGATE(FOnTaskComplete);
	FOnTaskComplete OnTaskComplete;

protected:

	UPROPERTY(EditAnywhere)
	bool bWaitForCompletion = false;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartTask();

	UFUNCTION(BlueprintCallable)
	virtual UWorld* BP_GetWorld() const;

protected:

	virtual UWorld* GetWorld() const override;

};


UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class RENEVENTFLOW_API UEventflowNodeExternalData : public UObject
{

	GENERATED_BODY()

};


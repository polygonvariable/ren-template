// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Kismet/BlueprintAsyncActionBase.h"

// Project Headers

// Generated Headers
#include "WaitForObjectiveCompletion.generated.h"

// Forward Declarations



/**
 *
 * 
 */
UCLASS(BlueprintType, Meta = (ExposedAsyncProxy = AsyncTask))
class UWaitForObjectiveCompletion : public UBlueprintAsyncActionBase
{

	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleted, UObject*, Target);
	UPROPERTY(BlueprintAssignable)
	FOnCompleted OnCompleted;

	UFUNCTION(BlueprintCallable, Meta = (BlueprintInternalUseOnly = "true"))
	static UWaitForObjectiveCompletion* WaitForObjectiveCompletion(UObject* InTarget);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UFUNCTION()
	void OnObjectiveCompleted();

protected:

	TWeakObjectPtr<UObject> Target;
	
protected:

	// ~ UBlueprintAsyncActionBase
	virtual void Activate() override;
	// ~ End of UBlueprintAsyncActionBase

};
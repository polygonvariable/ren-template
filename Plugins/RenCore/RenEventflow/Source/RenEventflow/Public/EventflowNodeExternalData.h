// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeExternalData.generated.h"

// Forward Declarations



UCLASS(Abstract, Blueprintable)
class RENEVENTFLOW_API UEventflowNodeTask : public UObject
{

	GENERATED_BODY()

public:

	bool GetWaitForCompletion() const;

	void StartAction();

	UFUNCTION(BlueprintCallable)
	void StopAction();

protected:

	UPROPERTY(EditAnywhere)
	bool bWaitForCompletion = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnActionStarted();

public:

	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR

	/*
	 * Allow editor to display world related nodes such as SpawnActor etc. for this class.
	 * Found this in thread: https://forums.unrealengine.com/t/how-to-make-a-blueprint-derived-from-a-uobject-class-access-world/366934/7
	 */
	virtual bool ImplementsGetWorld() const override;

#endif

public:

	DECLARE_MULTICAST_DELEGATE(FOnActionStopped);
	FOnActionStopped OnActionStopped;

};


UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class RENEVENTFLOW_API UEventflowNodeExternalData : public UObject
{

	GENERATED_BODY()

};


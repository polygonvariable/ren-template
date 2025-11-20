// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentController.generated.h"

// Forward Declarations
class UTimer;

class UPriorityList;
class UEnvironmentProfileAsset;



/**
 * 
 * 
 * 
 */
UCLASS(Abstract)
class UEnvironmentDiscreteController : public UObject
{

	GENERATED_BODY()

public:

	virtual void Initialize(AActor* Actor);
	virtual void Deinitialize();

};



/**
 * 
 * 
 * 
 */
UCLASS(Abstract)
class UEnvironmentStackedController : public UObject
{

	GENERATED_BODY()

public:

	EEnvironmentProfileType ProfileType;

	virtual void Initialize(AActor* Actor);
	virtual void Deinitialize();

	bool AddProfile(UEnvironmentProfileAsset* Profile, int Priority);
	bool RemoveProfile(int Priority);

protected:

	UPROPERTY()
	TObjectPtr<UPriorityList> PriorityList;

	// ~ Bindings
	virtual void HandleItemChanged(UObject* Item);
	// ~ End of Bindings

	// ~ Timer
	void StartTransition();

	virtual void HandleTimerTick(float ElapsedTime);

	float GetTransitionDuration() const;
	// ~ End of Timer

private:

	UPROPERTY()
	TObjectPtr<UTimer> Timer;

	float TransitionRate = 0.5f;
	float TransitionDuration = 1.0f;

	void SetTransitionRate(float InRate);
	void SetTransitionDuration(float InDuration);

};



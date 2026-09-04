// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"
#include "Priority/PriorityListInterface.h"

// Generated Headers
#include "EnvironmentController.generated.h"

// Forward Declarations
class UEnvironmentProfileAsset;


/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class UEnvironmentDiscreteController : public UObject
{

	GENERATED_BODY()

public:

	virtual void Initialize(AActor* Actor);
	virtual void Deinitialize();

};


/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class UEnvironmentStackedController : public UObject, public IPriorityListInterface
{

	GENERATED_BODY()

public:

	EEnvironmentProfileType ProfileType;


	virtual void Initialize(AActor* Actor);
	virtual void Deinitialize();

	bool AddProfile(UEnvironmentProfileAsset* Profile, int Priority);
	bool RemoveProfile(int Priority);

protected:

	FTimerHandle TimerHandle;
	TObjectPtr<UCurveFloat> TransitionCurve = nullptr;


	void StartTransition();
	void ClearTransition();
	virtual void OnTransitionChanged(float Alpha);

	// ~ IPriorityListInterface
	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() override final;
	virtual int& GetHighestPriority() override final;
	virtual void OnPriorityItemChanged(UObject* Item) override;
	// ~ End of IPriorityListInterface

private:

	UPROPERTY()
	TMap<int, TWeakObjectPtr<UObject>> _PriorityItems;
	int _HighestPriority = 0;

	float _TransitionRate = 0.5f;
	float _TransitionDuration = 1.0f;
	float _ElapsedTime = 0.0f;


	// ~ Binding
	void HandleOnTransitionTick();
	// ~ End of Binding

};


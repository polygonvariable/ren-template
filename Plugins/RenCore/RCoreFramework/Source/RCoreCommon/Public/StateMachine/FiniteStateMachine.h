// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/StateMachine.h"

// Generated Headers
#include "FiniteStateMachine.generated.h"

// Module Macros
#define REN_API RCORECOMMON_API


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UFiniteStateMachine : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_ThreeParams(FOnStateChanged, EFSMState /* Previous State */, EFSMState /* New State */, EFSMResult /* Result */);
	FOnStateChanged OnStateChanged;


	UFUNCTION(BlueprintCallable)
	REN_API void Initialize();

	UFUNCTION(BlueprintCallable)
	REN_API void Load();

	UFUNCTION(BlueprintCallable)
	REN_API void Execute();

	UFUNCTION(BlueprintCallable)
	REN_API void Finish(EFSMResult Result);

	UFUNCTION(BlueprintCallable)
	REN_API void Ready();

	UFUNCTION(BlueprintCallable)
	REN_API void Restart();

	UFUNCTION(BlueprintCallable)
	REN_API void Reset();

	REN_API EFSMResult GetResult() const;
	REN_API EFSMState GetState() const;

protected:

	// ~ Template
	REN_API virtual void OnInitialized(EFSMState PreviousState);
	REN_API virtual void OnLoaded(EFSMState PreviousState);
	REN_API virtual void OnReady(EFSMState PreviousState);

	REN_API virtual void OnActive(EFSMState PreviousState);
	REN_API virtual void OnEndActive(EFSMState NextState, EFSMResult Result);

	REN_API virtual void OnFinished(EFSMResult Result);
	REN_API virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult);
	REN_API virtual void OnReset();
	// ~ End of Template

private:

	EFSMState _CurrentState = EFSMState::Uninitialized;
	EFSMResult _CurrentResult = EFSMResult::None;

	TArray<FFSMTransition> _TransitionQueue;

	bool _bIsTransitioning = false;


	REN_API bool CanTransitionTo(EFSMState NextState, EFSMResult Result) const;
	REN_API bool SetState(EFSMState NextState, EFSMResult Result = EFSMResult::None);

};


// Module Macros
#undef REN_API


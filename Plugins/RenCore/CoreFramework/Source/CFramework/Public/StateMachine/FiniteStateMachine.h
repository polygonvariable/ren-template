// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "FiniteStateMachine.generated.h"


/**
 *
 */
UCLASS(Abstract)
class CFRAMEWORK_API UFiniteStateMachine : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_ThreeParams(FOnStateChanged, EFSMState /* Previous State */, EFSMState /* New State */, EFSMResult /* Result */);
	FOnStateChanged OnStateChanged;


	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintCallable)
	void Load();

	UFUNCTION(BlueprintCallable)
	void Execute();

	UFUNCTION(BlueprintCallable)
	void Active();

	UFUNCTION(BlueprintCallable)
	void Finish(EFSMResult Result);

	UFUNCTION(BlueprintCallable)
	void Ready();

	UFUNCTION(BlueprintCallable)
	void Restart();

	UFUNCTION(BlueprintCallable)
	void Reset();

	EFSMResult GetResult() const;
	EFSMState GetState() const;

protected:

	// ~ Template
	virtual void OnInitialized(EFSMState PreviousState);
	virtual void OnLoaded(EFSMState PreviousState);
	virtual void OnReady(EFSMState PreviousState);

	virtual void OnActive(EFSMState PreviousState);
	virtual void OnEndActive(EFSMState NextState, EFSMResult Result);

	virtual void OnFinished(EFSMResult Result);
	virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult);
	virtual void OnReset();
	// ~ End of Template

private:

	EFSMState _CurrentState = EFSMState::Uninitialized;
	EFSMResult _CurrentResult = EFSMResult::None;

	TArray<FFSMTransition> _TransitionQueue;

	bool _bIsTransitioning = false;


	bool CanTransitionTo(EFSMState NextState, EFSMResult Result) const;
	bool SetState(EFSMState NextState, EFSMResult Result = EFSMResult::None);

};


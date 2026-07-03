// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "StateMachine/FiniteStateMachine.h"

// Project Headers
#include "Definition/StateMachine.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UFiniteStateMachine::Initialize()
{
	SetState(EFSMState::Initialized);
}

void UFiniteStateMachine::Load()
{
	SetState(EFSMState::Loaded);
}

void UFiniteStateMachine::Ready()
{
	SetState(EFSMState::Ready);
}

void UFiniteStateMachine::Execute()
{
	SetState(EFSMState::Active);
}

void UFiniteStateMachine::Finish(EFSMResult Result)
{
	if (Result == EFSMResult::None)
	{
		LOG_WARNING(LogFSM, TEXT("Result is None, overriding it to Success"));
		Result = EFSMResult::Success;
	}
	SetState(EFSMState::Finished, Result);
}

void UFiniteStateMachine::Restart()
{
	SetState(EFSMState::Ready);
}

void UFiniteStateMachine::Reset()
{
	SetState(EFSMState::Uninitialized, EFSMResult::None);
}

bool UFiniteStateMachine::CanTransitionTo(EFSMState NextState, EFSMResult Result) const
{
	if (NextState == EFSMState::Uninitialized)
	{
		return _CurrentState != EFSMState::Uninitialized;
	}

	if (_CurrentState == EFSMState::Finished)
	{
		return NextState == EFSMState::Ready;
	}

	if (NextState == EFSMState::Finished)
	{
		if (_CurrentState == EFSMState::Uninitialized)
		{
			return false;
		}
		if (Result == EFSMResult::Success && _CurrentState != EFSMState::Active)
		{
			return false;
		}
		return true;
	}

	switch (_CurrentState)
	{
	case EFSMState::Uninitialized:
		return NextState == EFSMState::Initialized;
	case EFSMState::Initialized:
		return NextState == EFSMState::Loaded;
	case EFSMState::Loaded:
		return NextState == EFSMState::Ready;
	case EFSMState::Ready:
		return NextState == EFSMState::Active;
	case EFSMState::Active:
		return NextState == EFSMState::Ready;
	default:
		return false;
	}
}

EFSMResult UFiniteStateMachine::GetResult() const
{
	return _CurrentResult;
}

EFSMState UFiniteStateMachine::GetState() const
{
	return _CurrentState;
}

bool UFiniteStateMachine::SetState(EFSMState NextState, EFSMResult Result)
{
	_TransitionQueue.Add({ NextState, Result });
	if (_bIsTransitioning)
	{
		return true;
	}

	_bIsTransitioning = true;

	struct FFSMStateBroadcast
	{
		EFSMState PreviousState;
		EFSMState NextState;
		EFSMResult Result;
	};
	TArray<FFSMStateBroadcast> PendingBroadcasts;

	while (_TransitionQueue.Num() > 0)
	{
		FFSMTransition Target = _TransitionQueue[0];
		_TransitionQueue.RemoveAt(0);

		if (!CanTransitionTo(Target.State, Target.Result))
		{
			continue;
		}

		EFSMState PreviousStatus = _CurrentState;
		EFSMResult PreviousResult = _CurrentResult;

		if (PreviousStatus == EFSMState::Active)
		{
			EFSMResult ExitResult = (Target.State == EFSMState::Uninitialized || Target.State == EFSMState::Ready) ? EFSMResult::Aborted : Target.Result;
			OnEndActive(Target.State, ExitResult);
		}

		_CurrentState = Target.State;
		_CurrentResult = Target.Result;

		switch (_CurrentState)
		{
		case EFSMState::Uninitialized:
			OnReset();
			break;
		case EFSMState::Initialized:
			OnInitialized(PreviousStatus);
			break;
		case EFSMState::Loaded:
			OnLoaded(PreviousStatus);
			break;
		case EFSMState::Ready:
			if (PreviousStatus == EFSMState::Active || PreviousStatus == EFSMState::Finished)
			{
				OnRestart(PreviousStatus, PreviousResult);
			}
			OnReady(PreviousStatus);
			break;
		case EFSMState::Active:
			OnActive(PreviousStatus);
			break;
		case EFSMState::Finished:
			OnFinished(_CurrentResult);
			break;
		}

		PendingBroadcasts.Add({ PreviousStatus, _CurrentState, _CurrentResult });
	}

	_bIsTransitioning = false;

	for (const FFSMStateBroadcast& Broadcast : PendingBroadcasts)
	{
		OnStateChanged.ExecuteIfBound(Broadcast.PreviousState, Broadcast.NextState, Broadcast.Result);
	}

	return true;
}

void UFiniteStateMachine::OnInitialized(EFSMState PreviousState)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter initialized"));
}

void UFiniteStateMachine::OnLoaded(EFSMState PreviousState)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter loaded"));
}

void UFiniteStateMachine::OnReady(EFSMState PreviousState)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter ready"));
}

void UFiniteStateMachine::OnActive(EFSMState PreviousState)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter active"));
}

void UFiniteStateMachine::OnEndActive(EFSMState NextState, EFSMResult Result)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Exit active"));
}

void UFiniteStateMachine::OnFinished(EFSMResult Result)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter finished"));
}

void UFiniteStateMachine::OnRestart(EFSMState PreviousState, EFSMResult PreviousResult)
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter restart"));
}

void UFiniteStateMachine::OnReset()
{
	PRINT_SUCCESS(LogFSM, 1.0f, TEXT("Enter reset"));
}


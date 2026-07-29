// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/StateMachine.h"

// Generated Headers
#include "EventflowTransition.generated.h"


/**
 *
 */
UENUM()
enum class EEventflowTransitionType : uint8
{
	None UMETA(DisplayName = "None"),
	RestartNode UMETA(DisplayName = "Restart Node"),
	NextNode UMETA(DisplayName = "Next Node"),
	GraphFail UMETA(DisplayName = "Graph Fail"),
	GraphSuccess UMETA(DisplayName = "Graph Success")
};

/**
 *
 */
USTRUCT()
struct FEventflowTransition
{

	GENERATED_BODY()

public:

	FEventflowTransition() {};
	FEventflowTransition(EFSMResult InResult, EEventflowTransitionType InType) : Result(InResult), Type(InType) {};


	UPROPERTY(EditAnywhere)
	EFSMResult Result = EFSMResult::Success;

	UPROPERTY(EditAnywhere)
	EEventflowTransitionType Type = EEventflowTransitionType::NextNode;

};


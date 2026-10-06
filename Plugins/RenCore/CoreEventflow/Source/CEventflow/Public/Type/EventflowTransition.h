// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "EventflowTransition.generated.h"


/**
 *
 */
UENUM()
enum class EEventflowPrimaryTaskType : uint8
{
	Entry UMETA(DisplayName = "Entry"),
	Other UMETA(DisplayName = "Other"),
	Exit UMETA(DisplayName = "Exit")
};


/**
 *
 */
UENUM()
enum class EEventflowTransitionType : uint8
{
	None UMETA(DisplayName = "None"),
	RestartNode UMETA(DisplayName = "Restart Node"),
	NextNode UMETA(DisplayName = "Next Node"),
	RedirectNode UMETA(DisplayName = "Redirect Node"),
	GraphFail UMETA(DisplayName = "Graph Fail"),
	GraphSuccess UMETA(DisplayName = "Graph Success")
};

/**
 *
 */
USTRUCT(BlueprintType)
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

/**
 *
 */
USTRUCT()
struct FEventflowTransitionData
{

	GENERATED_BODY()

public:

	FEventflowTransitionData() {};

	UPROPERTY(EditAnywhere)
	EEventflowTransitionType Type = EEventflowTransitionType::NextNode;

	UPROPERTY(EditAnywhere)
	int NextNodeIndex = 0;

	UPROPERTY(EditAnywhere)
	FGuid NextNodeId;

};


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
enum class EEventflowNodeType : uint8
{
	Entry UMETA(DisplayName = "Entry"),
	Other UMETA(DisplayName = "Other"),
	Exit UMETA(DisplayName = "Exit")
};


/**
 *
 */
UENUM()
enum class EEventflowGraphTransitionType : uint8
{
	GraphFail UMETA(DisplayName = "Graph Fail"),
	GraphSuccess UMETA(DisplayName = "Graph Success"),
};

/**
 *
 */
UENUM()
enum class EEventflowNodeTransitionType : uint8
{
	None UMETA(DisplayName = "None"),
	RestartNode UMETA(DisplayName = "Restart Node"),
	NextNode UMETA(DisplayName = "Next Node"),
	RedirectNode UMETA(DisplayName = "Redirect Node"),
};


/**
 *
 */
USTRUCT()
struct FEventflowNodeTransitionData
{

	GENERATED_BODY()

public:

	FEventflowNodeTransitionData() {};

	UPROPERTY(EditAnywhere)
	EEventflowNodeTransitionType NodeTransition = EEventflowNodeTransitionType::NextNode;

	UPROPERTY(EditAnywhere)
	int NextNodeIndex = 0;

	UPROPERTY(EditAnywhere)
	FGuid NextNodeId;

};


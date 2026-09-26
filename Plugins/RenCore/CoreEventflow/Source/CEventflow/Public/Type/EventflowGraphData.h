// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EventflowGraphData.generated.h"

// Forward Declarations
class UEventflowPrimaryTask;


/**
 *
 */
USTRUCT()
struct FEventflowPinRelation
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FGuid LinkedToPin;

	UPROPERTY(VisibleAnywhere)
	FGuid LinkedToNode;

};

/**
 *
 */
USTRUCT()
struct FEventflowPin
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FGuid UniqueId;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Name (Ed)"))
	FName Name;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Category (Ed)"))
	FName Category;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Friendly Name (Ed)"))
	FText FriendlyName;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Constant (Ed)"))
	bool bIsConst = false;

#endif

};

/**
 *
 */
USTRUCT()
struct FEventflowNode
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TArray<FEventflowPin> StaticInputs;

	UPROPERTY(VisibleAnywhere)
	TArray<FEventflowPin> StaticOutputs;

	UPROPERTY(VisibleAnywhere, Instanced)
	TObjectPtr<UEventflowPrimaryTask> Task = nullptr;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Class (Ed)"))
	FName ClassName;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Position (Ed)"))
	FVector2D Position = FVector2D::Zero();

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Runtime Inputs (Ed)"))
	TArray<FText> RuntimeInputs;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Runtime Outputs (Ed)"))
	TArray<FText> RuntimeOutputs;

#endif

	void Reset()
	{
		StaticInputs.Empty();
		StaticOutputs.Empty();
		Task = nullptr;

#if WITH_EDITOR
		ClassName = NAME_None;
		Position = FVector2D::Zero();
		RuntimeInputs.Empty();
		RuntimeOutputs.Empty();
#endif
	}

};
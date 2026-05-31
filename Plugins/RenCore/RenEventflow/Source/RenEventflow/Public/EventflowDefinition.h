// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "InstancedStruct.h"

// Generated Headers
#include "EventflowDefinition.generated.h"

// Forward Declarations
class UEventflowTask;


UENUM()
enum class EEventflowDirection : uint8
{
	Next UMETA(DisplayName = "Next"),
	Previous UMETA(DisplayName = "Previous")
};


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
struct FEventflowPinDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FGuid UniqueId;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Name (Ed)"))
	FName Name;

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Category (Ed)"))
	FName Category;

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Friendly Name (Ed)"))
	FText FriendlyName;

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Constant (Ed)"))
	bool bIsConst = false;

#endif

};


/**
 *
 */
USTRUCT()
struct FEventflowNodeDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TArray<FEventflowPinDefinition> StaticInputs;

	UPROPERTY(VisibleAnywhere)
	TArray<FEventflowPinDefinition> StaticOutputs;

	UPROPERTY(VisibleAnywhere, Instanced)
	TObjectPtr<UEventflowTask> PrimaryTask = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<UEventflowTask>> SecondaryTasks;

#if WITH_EDITORONLY_DATA

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Class (Ed)"))
	FName ClassName;

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Position (Ed)"))
	FVector2D Position = FVector2D::Zero();

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Runtime Inputs (Ed)"))
	TArray<FText> RuntimeInputs;

	UPROPERTY(VisibleAnywhere, Meta = (DisplayName = "Runtime Outputs (Ed)"))
	TArray<FText> RuntimeOutputs;

#endif

	void Reset()
	{
		StaticInputs.Empty();
		StaticOutputs.Empty();
		PrimaryTask = nullptr;
		SecondaryTasks.Empty();

#if WITH_EDITOR
		ClassName = NAME_None;
		Position = FVector2D::Zero();
		RuntimeInputs.Empty();
		RuntimeOutputs.Empty();
#endif
	}

};

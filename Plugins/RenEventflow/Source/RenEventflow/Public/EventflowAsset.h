// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declarations




UENUM(BlueprintType)
enum EEventflowNodeType
{
	Start UMETA(DisplayName = "Start"),
	Action UMETA(DisplayName = "Action"),
	End UMETA(DisplayName = "End"),
};


UCLASS()
class RENEVENTFLOW_API UEventflowPin : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FText PinFriendlyName;

	UPROPERTY()
	FGuid PinGuid;

	UPROPERTY()
	bool bIsConst = false;

	UPROPERTY()
	TObjectPtr<class UEventflowPin> NextPin = nullptr;

};


UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class RENEVENTFLOW_API UEventflowNodeExternalData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	int Age;

};


UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowNodeData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(EditAnywhere)
	FText Content;

	UPROPERTY(EditAnywhere)
	TArray<FText> InputOptions;

	UPROPERTY(EditAnywhere)
	TArray<FText> OutputOptions;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UEventflowNodeExternalData>> ExternalData;

};


UCLASS()
class RENEVENTFLOW_API UEventflowNode : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid NodeGuid; // Runtime node id

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> InputPins;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> OutputPins;

	UPROPERTY()
	FString NodeType;

	UPROPERTY()
	FVector2D Position;

	UPROPERTY()
	TObjectPtr<UEventflowNodeData> NodeData = nullptr;

};


UCLASS()
class RENEVENTFLOW_API UEventflowData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<TObjectPtr<UEventflowNode>> Nodes;

};

UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowAsset : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString SomeString = "";

	UPROPERTY()
	TObjectPtr<UEventflowData> GraphData = nullptr;

};


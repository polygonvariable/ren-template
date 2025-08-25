// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/ObjectSaveContext.h"

// Project Headers

// Generated Headers
#include "EventflowAsset.generated.h"

// Forward Declarations



UCLASS()
class RENEVENTFLOW_API UEventflowPin : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FName PinCategory;

	UPROPERTY()
	FText PinFriendlyName;

	UPROPERTY()
	FGuid PinGuid;

	UPROPERTY()
	bool bPinIsConst = false;

	//UPROPERTY()
	//UEventflowNode* PinParentNode = nullptr;

	UPROPERTY()
	TObjectPtr<UEventflowPin> PinLinkedTo = nullptr;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

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

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};


UCLASS()
class RENEVENTFLOW_API UEventflowNode : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid NodeGuid;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> InputPins;

	UPROPERTY()
	TArray<TObjectPtr<UEventflowPin>> OutputPins;

	UPROPERTY()
	FName NodeType;

	UPROPERTY()
	FVector2D NodePosition;

	UPROPERTY()
	TObjectPtr<UEventflowNodeData> NodeData = nullptr;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};


UCLASS()
class RENEVENTFLOW_API UEventflowData : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<TObjectPtr<UEventflowNode>> Nodes;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};


UCLASS(BlueprintType)
class RENEVENTFLOW_API UEventflowAsset : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UObject> SharedData;

	UPROPERTY()
	TObjectPtr<UEventflowData> GraphData = nullptr;

	DECLARE_MULTICAST_DELEGATE(FOnEventflowAssetSaved);
	FOnEventflowAssetSaved OnEventflowAssetSaved;

protected:

	virtual void PreSaveRoot(FObjectPreSaveRootContext ObjectSaveContext) override;
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers
#include "EventflowNodeTask.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowNode;


/**
 *
 *
 */
UCLASS(Abstract, Blueprintable, MinimalAPI)
class UEventflowNodeTask : public UObject
{

	GENERATED_BODY()

public:

	RENEVENTFLOW_API void StartAction();

	UFUNCTION(BlueprintCallable)
	RENEVENTFLOW_API void StopAction(bool bSuccess);

	RENEVENTFLOW_API void SetOwnedNode(UEventflowNode* Node);
	RENEVENTFLOW_API void SetOwnedGraphAsset(UEventflowAsset* Asset);

	RENEVENTFLOW_API virtual void ResetData();

protected:

	TWeakObjectPtr<UEventflowNode> OwnedNode;
	TWeakObjectPtr<UEventflowAsset> OwnedGraphAsset;


	UFUNCTION(BlueprintNativeEvent)
	void OnActionStarted();
	RENEVENTFLOW_API virtual void OnActionStarted_Implementation();

public:

	RENEVENTFLOW_API virtual UWorld* GetWorld() const override;

#if WITH_EDITOR

	/*
	 * Allow editor to display world related nodes such as SpawnActor etc. for this class.
	 * Found this in thread: https://forums.unrealengine.com/t/how-to-make-a-blueprint-derived-from-a-uobject-class-access-world/366934/7
	 */
	RENEVENTFLOW_API virtual bool ImplementsGetWorld() const override;

#endif

public:

	DECLARE_DELEGATE_OneParam(FOnActionStopped, bool /* bSuccess */);
	FOnActionStopped OnActionStopped;

};


UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class RENEVENTFLOW_API UEventflowNodeExternalData : public UObject
{

	GENERATED_BODY()

};


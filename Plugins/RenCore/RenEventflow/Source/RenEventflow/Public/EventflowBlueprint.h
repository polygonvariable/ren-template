// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RenEventflow/Public/EventflowDelegates.h"

// Generated Headers
#include "EventflowBlueprint.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowNode;



/**
 *
 *
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class RENEVENTFLOW_API UEventflowBlueprint : public UObject
{

	GENERATED_BODY()

public:

	bool StartNodeExecution(UEventflowNode* Node);

	virtual void RegisterBlueprint(UEventflowAsset* Asset);
	virtual void UnregisterBlueprint();

	FOnNodeExited OnNodeExited;

public:

	// ~ UObject
	virtual UWorld* GetWorld() const override;

#if WITH_EDITOR

	/*
	 * Allow editor to display world related nodes such as SpawnActor etc. for this class.
	 * Found this in thread: https://forums.unrealengine.com/t/how-to-make-a-blueprint-derived-from-a-uobject-class-access-world/366934/7
	 */
	virtual bool ImplementsGetWorld() const override;

#endif
	// ~ End of UObject

protected:

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UEventflowAsset> EventflowAsset;

	UFUNCTION(BlueprintCallable)
	virtual void FinishNodeExecution(UEventflowNode* Node, bool bSuccess, int NextNodeIndex);

#if WITH_EDITOR

public:

	static bool HasValidSignature(UFunction* Function, const TArray<FFieldClass*>& ExpectedTypes);

#endif

};


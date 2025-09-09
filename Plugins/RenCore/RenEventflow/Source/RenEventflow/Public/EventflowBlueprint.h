// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "EventflowBlueprint.generated.h"

// Forward Declarations
class UEventflowAsset;
class UEventflowNode;



DECLARE_DELEGATE_TwoParams(FOnNodeExecuteFinished, UEventflowNode*, bool);



/*
UINTERFACE(MinimalAPI)
class UEventflowBlueprintInterface : public UInterface
{

	GENERATED_BODY()

};
class IEventflowBlueprintInterface
{

	GENERATED_BODY()

public:

	virtual bool StartNodeExecution(UEventflowNode* Node) = 0;
	virtual void RegisterBlueprint(UEventflowAsset* Asset) = 0;
	virtual void UnregisterBlueprint() = 0;

	virtual FOnNodeExecuteFinished& GetOnNodeExecuteFinished() = 0;

protected:

	virtual void FinishNodeExecution(UEventflowNode* Node, bool bSuccess) = 0;

};
*/


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

	FOnNodeExecuteFinished OnNodeExecuteFinished;

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
	virtual void FinishNodeExecution(UEventflowNode* Node, bool bSuccess);

#if WITH_EDITOR

public:

	static bool HasValidSignature(UFunction* Function, const TArray<FFieldClass*>& ExpectedTypes);

#endif

};


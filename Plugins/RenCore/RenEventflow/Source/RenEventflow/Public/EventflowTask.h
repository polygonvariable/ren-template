// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "EventflowDefinition.h"

// Generated Headers
#include "EventflowTask.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

// Forward Declarations
class UEventflowAsset;
class UEventflowEngine;
struct FEventflowNodeDefinition;


/**
 *
 */
UCLASS(Abstract, MinimalAPI, EditInlineNew, DefaultToInstanced)
class UEventflowTask : public UObject
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_TwoParams(FOnTaskFinished, EEventflowDirection /*Flow Direction*/, int /* Flow Index */);
	FOnTaskFinished OnTaskFinished;

	REN_API virtual void CopyFromTemplate(const UEventflowTask* Template) {};

	REN_API virtual void InitializeTask(const FGuid& NodeId, const FEventflowNodeDefinition* Node);
	REN_API virtual void DeinitializeTask();

	// ~ UObject
	REN_API virtual UWorld* GetWorld() const override;
	// ~ End of UObject

#if WITH_EDITOR

	/*
	 * Allow editor to display world related nodes such as SpawnActor etc. for this class.
	 * Found this in thread: https://forums.unrealengine.com/t/how-to-make-a-blueprint-derived-from-a-uobject-class-access-world/366934/7
	 */
	// ~ UObject
	REN_API virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

	REN_API virtual TArray<FText> GetRuntimeInputs() const;
	REN_API virtual TArray<FText> GetRuntimeOutputs() const;

#endif

protected:

	FGuid CurrentNodeId;
	const FEventflowNodeDefinition* CurrentNode = nullptr;


	REN_API virtual void Initialization();
	REN_API virtual void Deinitialization();

	UFUNCTION(BlueprintCallable)
	REN_API void FinishTask(EEventflowDirection Direction, int Index);

	UFUNCTION(BlueprintCallable)
	REN_API bool IsActive() const;


	REN_API UEventflowEngine* GetOwningEngine() const;

	template<class T>
	T* GetOwningEngine() const
	{
		return Cast<T>(GetOwningEngine());
	}

private:

	bool _bIsActive = false;

};


// Module Macros
#undef REN_API


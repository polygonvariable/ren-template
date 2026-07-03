// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "StateMachine/FiniteStateMachine.h"

// Generated Headers
#include "EventflowTask.generated.h"

// Module Macros
#define REN_API RENEVENTFLOW_API

// Forward Declarations
class UEventflowEngine;


/**
 *
 */
UCLASS(Abstract, MinimalAPI, EditInlineNew, DefaultToInstanced, CollapseCategories)
class UEventflowTask : public UFiniteStateMachine
{

	GENERATED_BODY()

public:

	REN_API virtual void CopyFromAsset(const UEventflowTask* Template);

	// ~ UObject
	REN_API virtual UWorld* GetWorld() const override;
	// ~ End of UObject

#if WITH_EDITOR

	REN_API virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData);

	// ~ UObject
	REN_API virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

#endif

protected:

	REN_API UEventflowEngine* GetOwningEngine() const;

	template<class T>
	T* GetOwningEngine() const
	{
		return Cast<T>(GetOwningEngine());
	}

};


// Module Macros
#undef REN_API


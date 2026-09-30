// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "StateMachine/FiniteStateMachine.h"

// Generated Headers
#include "EventflowTask.generated.h"

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

	CEVENTFLOW_API virtual void CopyFromAsset(const UEventflowTask* Template);

	// ~ UObject
	CEVENTFLOW_API virtual UWorld* GetWorld() const override;
	// ~ End of UObject

#if WITH_EDITOR

	CEVENTFLOW_API virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData);

	// ~ UObject
	CEVENTFLOW_API virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

#endif

protected:

	CEVENTFLOW_API UEventflowEngine* GetOwningEngine() const;

	template<class T>
	T* GetOwningEngine() const
	{
		return Cast<T>(GetOwningEngine());
	}

};


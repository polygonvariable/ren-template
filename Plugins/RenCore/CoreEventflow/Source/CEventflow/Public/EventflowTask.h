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
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories)
class CEVENTFLOW_API UEventflowTask : public UFiniteStateMachine
{

	GENERATED_BODY()

public:

	virtual void CopyFromAsset(const UEventflowTask* Template);

	// ~ UObject
	virtual UWorld* GetWorld() const override;
	// ~ End of UObject

#if WITH_EDITOR
	virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData);

	// ~ UObject
	virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject
#endif

#if UE_BUILD_DEVELOPMENT
	virtual void GetEditorDebugInfo(TArray<FString>& OutDebug) const;
#endif

protected:

	UEventflowEngine* GetOwningEngine() const;

	template<class T>
	T* GetOwningEngine() const
	{
		return Cast<T>(GetOwningEngine());
	}

};


/**
 *
 */
UCLASS(Abstract)
class CEVENTFLOW_API UEventflowExternalReference : public UObject
{

	GENERATED_BODY()

};


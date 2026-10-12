// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/ObjectiveGlobalTaskFragment.h"
#include "System/Flow/Task/QuestGlobalTask.h"

// Generated Headers
#include "QuestGlobalTask_SpawnActor.generated.h"

// Forward Declarations
class UQuestEngine;


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestGlobalTask_SpawnActor : public UQuestGlobalTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Marker")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category = "Marker")
	FTransform ActorTransform;


	void UpdateActorTransform(FTransform Transform);

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask
#endif

protected:

	UPROPERTY(Transient)
	TObjectPtr<AActor> Actor = nullptr;


	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};


/*
 *
 */
USTRUCT(DisplayName = "Update Actor Transform")
struct FOGTFragment_UpdateActorTransform : public FObjectiveGlobalTaskFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	// ~ FObjectiveGlobalTaskFragment
	virtual void Execute(UQuestEngine* Engine) override;
	// ~ End of FObjectiveGlobalTaskFragment

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/ActorSpawnData.h"
#include "System/Flow/Task/QuestSubTask.h"

// Generated Headers
#include "QuestTask_GoToLocation.generated.h"

// Forward Declarations
class AQuestObjectiveActor;
class UEventflowTask;


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Go To Location"))
class UQuestTask_GoToLocation : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FActorSpawnData ActorData;


	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if WITH_EDITOR

	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual void PreDuplicate(FObjectDuplicationParameters& DupParams) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// ~ End of UObject

#endif

protected:

	UPROPERTY()
	TObjectPtr<AQuestObjectiveActor> RuntimeActor = nullptr;


	// ~ Bindings
	void HandleOnDestinationReached();
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;

	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnEndActive(EFSMState NextState, EFSMResult Result) override;

	virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};


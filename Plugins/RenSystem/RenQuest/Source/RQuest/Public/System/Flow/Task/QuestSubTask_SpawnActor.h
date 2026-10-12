// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"
#include "System/Flow/Task/QuestSubTask.h"

// Generated Headers
#include "QuestSubTask_SpawnActor.generated.h"

// Forward Declarations
class AQuestObjectiveMarker;


/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Spawn Actor"))
class UQuestTask_SpawnLocation : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UQuestTask_SpawnLocation();

	UPROPERTY(EditAnywhere, Category = "Marker")
	TSoftClassPtr<AQuestObjectiveMarker> MarkerClass;

	UPROPERTY(EditAnywhere, Category = "Marker")
	FTransform MarkerTransform;

	UPROPERTY(EditAnywhere, Category = "Marker", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FComponentTemplateDefinition>> MarkerComponents;


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
	TObjectPtr<AQuestObjectiveMarker> MarkerActor = nullptr;


	// ~ Binding
	virtual void HandleOnInteractionCompleted(EFSMResult Result);
	// ~ End of Binding

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};


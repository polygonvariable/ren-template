// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "ComponentTemplateDefinition.h"
#include "LuauSourceCode.h"
#include "System/Flow/Task/QuestPrimaryTask.h"

// Generated Headers
#include "QuestTask_Marker.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class AQuestObjectiveMarker;


/**
 * Trigger zone, auto activates when player enter its bounds
 */
UCLASS(MinimalAPI)
class UQuestTask_SpawnMarker : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

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
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

};


/**
 * Trigger zone, auto activates when player enter its bounds
 */
UCLASS(MinimalAPI)
class UQuestTask_ConditionalSpawnMarker : public UQuestTask_SpawnMarker
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Marker Condition", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FQuestConditionDefinition>> Conditions;

	UPROPERTY(EditAnywhere, Category = "Marker Condition")
	FLuauSourceCode LuauCode;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Marker Condition")
	bool bAutoCompile = true;
#endif

	// ~ Binding
	virtual void HandleOnInteractionCompleted(EFSMResult Result) override;
	// ~ End of Binding

};


/*
 *
 */
USTRUCT()
struct FQuestConditionDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName Name;

	virtual bool Evaluate(UWorld* World, UQuestPrimaryTask* Task) const;
	virtual ~FQuestConditionDefinition() = default;

};

/*
 *
 */
USTRUCT(DisplayName = "Actor Have Tag")
struct FQCondition_ActorHaveTag : public FQuestConditionDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName Tag;

	// ~ FQuestConditionDefinition
	virtual bool Evaluate(UWorld* World, UQuestPrimaryTask* Task) const override;
	// ~ End of FQuestConditionDefinition

};


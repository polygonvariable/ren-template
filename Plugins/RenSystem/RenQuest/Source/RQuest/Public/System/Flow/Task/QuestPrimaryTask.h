// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/ActorSpawnData.h"
#include "LuauSourceCode.h"
#include "Task/EventflowPrimaryTask.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "Core/Type/ComponentDefinition.h"

// Generated Headers
#include "QuestPrimaryTask.generated.h"

class FObjectPreSaveContext;
class UEventflowAsset;
class AQuestObjectiveMarker;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UQuestPrimaryTask : public UEventflowPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestPrimaryTask();


	UPROPERTY(EditAnywhere, Category = "Objective")
	FName Summary;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bIsTransient = true;

protected:

	int NextPinIndex = 0;

};



/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "External Task"))
class UQuestTask_ExternalTask : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "External Task")
	TObjectPtr<UEventflowAsset> ExternalTask;

	UPROPERTY(VisibleAnywhere, Category = "External Task")
	TArray<FGuid> ExternalPinIds;

	UPROPERTY(VisibleAnywhere, Category = "External Task")
	TArray<FName> ExternalPinTitles;

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

};


UENUM()
enum class ERerouteType : uint8
{
	Source, /* That sends signal */
	Target /* recieve the reroute signal */
};

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Reroute"))
class UQuestTask_Reroute : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_Reroute();


	UPROPERTY(EditAnywhere, Category = "Reroute")
	ERerouteType RerouteType;

	UPROPERTY(EditAnywhere, Category = "Reroute")
	FString RerouteName;

	UPROPERTY(VisibleAnywhere, Category = "Reroute", meta = (EditCondition = "RerouteType==ERerouteType::Source", EditConditionHides))
	FGuid RerouteId;


	virtual TInstancedStruct<FEventflowTransitionData>& GetTransitionData(EFSMResult Result) override;


	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_Begin : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_Begin();

	UPROPERTY(EditAnywhere, Category = "Quest")
	bool bShowSplash;

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};

/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_End : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_End();

	UPROPERTY(EditAnywhere, Category = "Quest")
	bool bShowSplash;

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};










/**
 * Trigger zone, auto activates when player enter its bounds
 */
UCLASS(MinimalAPI)
class UQuestTask_SpawnMarker : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_SpawnMarker();

	UPROPERTY(EditAnywhere, Category = "Marker")
	TSoftClassPtr<AQuestObjectiveMarker> MarkerClass;

	UPROPERTY(EditAnywhere, Category = "Marker")
	FTransform MarkerTransform;


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


	// ~ Bindings
	virtual void HandleOnInteractionCompleted();
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine

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

	// ~ Bindings
	virtual void HandleOnInteractionCompleted() override;
	// ~ End of Bindings

};





















/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Reach Location"))
class UQuestTask_ReachLocation : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Zone")
	FActorSpawnData ReachArea;

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
	TObjectPtr<AQuestObjectiveActor> RuntimeActor = nullptr;


	// ~ Binding
	virtual void HandleOnDestinationReached(EFSMResult Result);
	// ~ End of Binding

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;

	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnEndActive(EFSMState NextState, EFSMResult Result) override;

	virtual void OnFinished(EFSMResult Result) override;
	virtual void OnRestart(EFSMState PreviousState, EFSMResult PreviousResult) override;
	virtual void OnReset() override;
	// ~ End of UFiniteStateMachine
};

/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Countdown"))
class UQuestTask_Countdown : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Duration = 10.0f;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if UE_BUILD_DEVELOPMENT
	// ~ UEventflowTask
	virtual void GetEditorDebugInfo(TArray<FString>& OutDebug) const override;
	// ~ End of UEventflowTask
#endif

protected:

	FTimerHandle TimerHandle;

	// ~ Bindings
	void HandleCountdownOver();
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





/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Condition - Actor have tag"))
class UQuestTask_ActorHaveTag : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName Tag;

	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UFiniteStateMachine

};

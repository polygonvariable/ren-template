// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "ComponentDefinition.h"
#include "Core/Type/ActorSpawnData.h"
#include "LuauSourceCode.h"
#include "System/Flow/Task/QuestSubTask.h"
#include "Task/EventflowNodeTask.h"
#include "Task/EventflowGlobalTask.h"
#include "GameplayContextAction.h"
#include "Actor/QuestObjectiveActor.h"

// Generated Headers
#include "QuestPrimaryTask.generated.h"

class FObjectPreSaveContext;
class UEventflowAsset;
class AQuestObjectiveMarker;
class UQuestEngine;
class UUserWidget;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UQuestGlobalTask : public UEventflowGlobalTask
{
	GENERATED_BODY()
};




/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UQuestPrimaryTask : public UEventflowNodeTask
{

	GENERATED_BODY()

public:

	UQuestPrimaryTask();

	UPROPERTY(EditAnywhere, Category = "Objective")
	FName Summary;

	UPROPERTY(EditAnywhere, Category = "Objective")
	bool bIsTransient = true;

};



/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_ExternalTask : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "External Asset")
	TSoftObjectPtr<UEventflowAsset> ExternalAsset;

	UPROPERTY(VisibleAnywhere, Category = "External Asset")
	TArray<FGuid> ExternalPins;


	// ~ UEventflowTask
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask

	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	// ~ Binding
	void HandleOnEngineRemoved(FPrimaryAssetId AssetId, UEventflowEngine* Engine);
	// ~ End of Binding

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UEventflowTask

};


/**
 *
 */
UENUM()
enum class ERerouteType : uint8
{
	Source, /* That sends signal */
	Target, /* recieve the reroute signal */
};

/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_Reroute : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Reroute")
	ERerouteType RerouteType;

	UPROPERTY(EditAnywhere, Category = "Reroute")
	FString RerouteName;

	UPROPERTY(VisibleAnywhere, Category = "Reroute", meta = (EditCondition = "RerouteType==ERerouteType::Source", EditConditionHides))
	FGuid RerouteId;


	// ~ UEventflowNodeTask
	virtual TInstancedStruct<FEventflowNodeTransitionData>& GetTransitionData(EFSMResult Result) override;
	// ~ End of UEventflowNodeTask

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

	UPROPERTY(EditAnywhere, Category = "Graph")
	EEventflowGraphTransitionType GraphResult = EEventflowGraphTransitionType::GraphSuccess;

	// ~ UEventflowTask
	virtual void GetReturnData(TInstancedStruct<FEventflowReturnData>& ReturnData) override;
	virtual void CopyFromAsset(const UEventflowTask* Template) override;
	// ~ End of UEventflowTask

protected:

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	// ~ End of UEventflowTask

};





/*
 *
 */
USTRUCT()
struct FObjectiveGlobalTaskFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName TaskName;

	virtual void Execute(UQuestEngine* Engine);
	virtual ~FObjectiveGlobalTaskFragment() = default;

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

	// ~ FQuestGlobalTask
	virtual void Execute(UQuestEngine* Engine) override;
	// ~ End of FQuestGlobalTask

};




/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_EnsureGlobalTask : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Global Task", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveGlobalTaskFragment>> GlobalTasks;

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

	UPROPERTY(EditAnywhere, Category = "Marker")
	TArray<FComponentDefinition> MarkerComponents;


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






/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestTask_SubtaskGate : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

	UQuestTask_SubtaskGate();

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY(EditAnywhere, Category = "Subtask Condition")
	FLuauSourceCode LuauCode;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Subtask Condition")
	bool bAutoCompile = true;
#endif

	// ~ Binding
	virtual void HandleOnSubTaskStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result) override;
	// ~ End of Bindings

	// ~ UFiniteStateMachine
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
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

	// ~ Binding
	virtual void HandleOnInteractionCompleted(EFSMResult Result) override;
	// ~ End of Binding

};









/**
 *
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Spawn Location"))
class UQuestTask_SpawnLocation : public UQuestSubTask
{

	GENERATED_BODY()

public:

	UQuestTask_SpawnLocation();

	UPROPERTY(EditAnywhere, Category = "Marker")
	TSoftClassPtr<AQuestObjectiveMarker> MarkerClass;

	UPROPERTY(EditAnywhere, Category = "Marker")
	FTransform MarkerTransform;

	UPROPERTY(EditAnywhere, Category = "Marker")
	TArray<FComponentDefinition> MarkerComponents;


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

	// ~ Binding
	void HandleCountdownOver();
	// ~ End of Binding

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
 * Trigger zone, auto activates when player enter its bounds
 */
UCLASS(MinimalAPI)
class UQuestGlobalTask_SpawnActor : public UQuestGlobalTask
{

	GENERATED_BODY()

public:

	UQuestGlobalTask_SpawnActor();

	UPROPERTY(EditAnywhere, Category = "Marker")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category = "Marker")
	FTransform ActorTransform;


	void UpdateTransform(FTransform Transform);

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
USTRUCT(DisplayName = "Start Quest")
struct FGameplayContext_StartQuest : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (AllowedTypes = "Quest"))
	FPrimaryAssetId AssetId;

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Owner, UObject* Instigator) override;
	// ~ End of FGameplayContextAction

};










/**
 *
 */
UCLASS(MinimalAPI)
class UQuestTask_CheckStorage : public UQuestPrimaryTask
{

	GENERATED_BODY()

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
class UQuestTask_WidgetGate : public UQuestPrimaryTask
{

	GENERATED_BODY()

public:

#if WITH_EDITOR
	// ~ UEventflowTask
	virtual void AppendAssetBundleData(FAssetBundleData& AssetBundle) override;
	// ~ End of UEventflowTask
	// ~ UObject
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY(EditAnywhere)
	FName WidgetGameplayMode;

	UPROPERTY(EditAnywhere, meta = (ExcludeBaseStruct))
	TInstancedStruct<FWidgetTemplateDefinition> WidgetTemplate;

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget;


	virtual void CopyFromAsset(const UEventflowTask* Template) override;

	// ~ UEventflowTask
	virtual void OnInitialized(EFSMState PreviousState) override;
	virtual void OnLoaded(EFSMState PreviousState) override;
	virtual void OnReady(EFSMState PreviousState) override;
	virtual void OnActive(EFSMState PreviousState) override;
	virtual void OnReset() override;
	// ~ End of UEventflowTask

	// ~ Binding
	virtual void HandleOnObjectiveFeedback(EFSMResult Result);
	// ~ End of Binding
};



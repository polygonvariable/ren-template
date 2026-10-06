// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Project Headers
#include "RegionActor.h"
#include "StateMachine/FiniteStateMachineType.h"
#include "GameplayContextAction.h"

// Generated Headers
#include "QuestObjectiveActor.generated.h"

// Forward Declarations
class UEventflowTask;


/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveActor : public ARegionActor
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TWeakObjectPtr<UEventflowTask> OwningTask;

	DECLARE_DELEGATE(FOnQuestObjectiveStarted);
	FOnQuestObjectiveStarted OnStarted;

	DECLARE_DELEGATE_OneParam(FOnQuestObjectiveCompleted, EFSMResult /* State */);
	FOnQuestObjectiveCompleted OnCompleted;




	void SetOwningTask(UEventflowTask* Task);


	UFUNCTION(BlueprintCallable)
	void StartTasks();

	UFUNCTION(BlueprintCallable)
	void AbortTasks();

	UFUNCTION(BlueprintCallable)
	void CompleteTasks();

	UFUNCTION(BlueprintNativeEvent)
	void OnLoaded();
	virtual void OnLoaded_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void OnActive();
	virtual void OnActive_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void OnAborted();
	virtual void OnAborted_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void OnSuccess();
	virtual void OnSuccess_Implementation() {};


	UFUNCTION(BlueprintCallable)
	void CompleteObjective(bool bSuccess);


protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;


};









/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveMarker : public ARegionActor
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE(FOnInteractionCompleted);
	FOnInteractionCompleted OnInteractionCompleted;

protected:

	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

};


/*
 *
 */
UCLASS(Abstract)
class AQuestInteractionMarker : public AQuestObjectiveMarker
{

	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (Categories = "GameContext"))
	FGameplayTagContainer ContextTags;

};




/*
 *
 */
USTRUCT(DisplayName = "Quest Interact Handle (Internal)")
struct FGameplayContext_QuestInteractionHandle : public FGameplayContextAction
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AQuestInteractionMarker> Owner = nullptr;

	// ~ FGameplayContextAction
	virtual bool Execute(UWorld* World, UObject* Caller) override;
	// ~ End of FGameplayContextAction

};
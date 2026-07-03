// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"

// Generated Headers
#include "QuestObjectiveActor.generated.h"

// Forward Declarations
class UEventflowTask;


/*
 *
 */
UCLASS(Abstract)
class AQuestObjectiveActor : public AActor
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TWeakObjectPtr<UEventflowTask> OwningTask;

	DECLARE_DELEGATE(FOnQuestObjectiveCompleted);
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

};


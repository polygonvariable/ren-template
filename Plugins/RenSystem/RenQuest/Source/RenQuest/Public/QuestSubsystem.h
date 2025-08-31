// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers

// Generated Headers
#include "QuestSubsystem.generated.h"

// Forward Declarations
class UQuestGraph;
class UStoryboardGraph;
class UEventflowNodeTask;
class UEventflowBlueprint;



UCLASS()
class UObjectPoolX : public UObject
{

	GENERATED_BODY()

public:

	void InitializePool(TSubclassOf<UObject> InClass, int32 InitialSize);

	UObject* Acquire();

	template <typename T>
	T* Acquire()
	{
		UObject* Obj = Acquire();
		return Cast<T>(Obj);
	}

	void Release(UObject* Object);

private:

	UPROPERTY()
	TSubclassOf<UObject> PooledClass;

	UPROPERTY()
	TArray<UObject*> AvailableObjects;

	UObject* CreateNewObject();
};



UCLASS()
class UQuestSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	void StartQuest(UQuestGraph* QuestGraph);
	void EndQuest(UQuestGraph* QuestGraph);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStarted, UQuestGraph*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, UQuestGraph*);

	FOnQuestStarted OnQuestStarted;
	FOnQuestCompleted OnQuestCompleted;


	bool IsQuestActive(UQuestGraph* QuestGraph) const;
	bool IsQuestCompleted(UQuestGraph* QuestGraph) const;

protected:

	// Map of active quests with objective index
	TMap<UQuestGraph*, TArray<int>> ActiveQuests;

	TArray<UQuestGraph*> CompletedQuests;

	TMultiMap<UQuestGraph*, UEventflowNodeTask*> QuestTasks;
	TMultiMap<UQuestGraph*, UEventflowBlueprint*> QuestBlueprints;

protected:

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};


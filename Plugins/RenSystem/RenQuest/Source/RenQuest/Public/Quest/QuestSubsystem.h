// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
// #include "GameFramework/Actor.h"
// #include "Components/ActorComponent.h"
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "RenQuest/Public/Quest/QuestDelegate.h"

// Generated Headers
#include "QuestSubsystem.generated.h"

// Forward Declarations
class UEventflowEngine;

class UQuestAsset;
class UQuestEngine;

class IQuestProviderInterface;
struct FQuestRecord;



/**
 *
 *
 */
UCLASS()
class UQuestSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	FOnQuestStarted OnQuestStarted;
	FOnQuestResumed OnQuestResumed;
	FOnQuestCompleted OnQuestCompleted;


	UFUNCTION(BlueprintCallable)
	void StartQuest(UQuestAsset* QuestAsset);
	void StartQuest(FGuid QuestId);

	void ResumeQuest(UQuestAsset* QuestAsset, FGuid EntryId);

	bool SetObjectiveToCompleted(UQuestAsset* QuestAsset, FGuid ObjectiveId);
	bool SetObjectiveToActive(UQuestAsset* QuestAsset, FGuid ObjectiveId);

	void EndQuest(UQuestAsset* QuestAsset);


	bool IsQuestActive(UQuestAsset* QuestAsset) const;
	bool IsQuestCompleted(UQuestAsset* QuestAsset) const;

	UFUNCTION(BlueprintCallable)
	void AddAvailableQuest(UQuestAsset* QuestAsset);


	FQuestRecord* GetActiveQuest(UQuestAsset* QuestAsset) const;
	FDateTime* GetCompletedQuest(UQuestAsset* QuestAsset) const;

	UFUNCTION(BlueprintCallable)
	bool IsObjectiveCompleted(UQuestAsset* QuestAsset, FGuid ObjectiveId) const;

	UFUNCTION(BlueprintCallable)
	bool IsObjectiveActive(UQuestAsset* QuestAsset, FGuid ObjectiveId) const;
	//TObjectPtr<UQuestAsset> FocusedQuest;

	const TMap<FGuid, FQuestRecord>* GetActiveQuests() const;
	const TMap<FGuid, FDateTime>* GetCompletedQuests() const;
	
protected:

	TMap<TObjectPtr<UQuestAsset>, TObjectPtr<UQuestEngine>> QuestEngines;

	TWeakInterfacePtr<IQuestProviderInterface> QuestProvider;

	TMap<FGuid, TObjectPtr<UQuestAsset>> AvailableQuests;

	UQuestAsset* GetQuestAsset(FGuid QuestId) const;

	TMap<FGuid, FQuestRecord>* GetMutableActiveQuests() const;
	TMap<FGuid, FDateTime>* GetMutableCompletedQuests() const;

	UFUNCTION(BlueprintCallable)
	void HandleStorageLoaded();

	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void Deinitialize() override;

};




/*
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
class AQuestNetworkSync : public AActor
{

	GENERATED_BODY()

public:

	AQuestNetworkSync();

};
UCLASS(Abstract, Blueprintable, MinimalAPI)
class UNetworkRouterComponent : public UActorComponent
{

	GENERATED_BODY()

};
UCLASS(Blueprintable, MinimalAPI, ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class UQuestNetworkRouter : public UNetworkRouterComponent
{

	GENERATED_BODY()

};
UCLASS(ClassGroup = (Custom), MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UNetworkSyncComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	AActor* GetNetworkActor(TSubclassOf<AActor> InClass);

	template<typename T>
	T* GetNetworkActor()
	{
		return Cast<T>(GetNetworkActor(T::StaticClass()));
	}

protected:

	UPROPERTY()
	TMap<UClass*, TWeakObjectPtr<AActor>> CachedNetworkActors;

};
UCLASS()
class UNetworkBlueprintLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UWorld* GetWorldObject(UObject* Object);

	UFUNCTION(BlueprintCallable)
	static UNetworkSyncComponent* GetNetworkSync(UWorld* WorldContextObject, TSubclassOf<UNetworkSyncComponent> InClass);

	UFUNCTION(BlueprintCallable)
	static UNetworkRouterComponent* GetNetworkRouter(UWorld* WorldContextObject, TSubclassOf<UNetworkRouterComponent> InClass);

	template<typename T>
	static T* GetNetworkRouter(UWorld* WorldContext)
	{
		return Cast<T>(GetNetworkRouter(WorldContext, T::StaticClass()));
	}

};
*/




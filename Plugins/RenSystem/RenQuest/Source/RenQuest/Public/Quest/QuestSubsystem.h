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



/*
 *
 *
 */
UCLASS()
class UQuestSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartQuest(UQuestAsset* QuestAsset);

	void EndQuest(UQuestAsset* QuestAsset);

	FOnQuestStarted OnQuestStarted;
	FOnQuestCompleted OnQuestCompleted;

	bool IsQuestActive(UQuestAsset* QuestAsset) const;
	bool IsQuestCompleted(UQuestAsset* QuestAsset) const;

	TMap<TObjectPtr<UQuestAsset>, TObjectPtr<UEventflowEngine>> ActiveQuests;
	TObjectPtr<UQuestAsset> FocusedQuest;

protected:

protected:

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




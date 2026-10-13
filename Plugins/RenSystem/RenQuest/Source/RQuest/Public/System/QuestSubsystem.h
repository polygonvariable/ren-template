// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/WorldSubsystem.h"

// Project Headers
#include "EventflowEngineProvider.h"
#include "StateMachine/FiniteStateMachineType.h"

// Generated Headers
#include "QuestSubsystem.generated.h"

// Forward Declarations
class UQuestStorageManager;
class UQuestEngine;
class AQuestActor;
class IStorageProvider;


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestSubsystem : public UWorldSubsystem, public IEventflowEngineProvider
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnQuestDataChanged);
	FOnQuestDataChanged OnQuestDataChanged;

	DECLARE_MULTICAST_DELEGATE(FOnQuestFocusChanged);
	FOnQuestFocusChanged OnQuestFocusChanged;

	UQuestStorageManager* GetStorageManager() const;


	UFUNCTION(BlueprintCallable)
	void StartQuest(const FPrimaryAssetId& AssetId, const TInstancedStruct<FEventflowEntryData>& EntryData);

	void FocusQuest(FPrimaryAssetId AssetId);
	void CancelFocusedTask();
	void CancelQuestTask(FPrimaryAssetId AssetId);
	void CancelQuestTask(FPrimaryAssetId AssetId, bool bRecursive);

	UQuestEngine* GetFocusedQuest() const;
	const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GetQuests() const;
	void GetQuestsByGroup(const FPrimaryAssetId& InAssetId, TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& OutQuests) const;

	// ~ IEventflowEngineProvider
	virtual void StartEventflow(const FPrimaryAssetId& AssetId, const TInstancedStruct<FEventflowEntryData>& EntryData) override;
	virtual void StopEventflow(const FPrimaryAssetId& AssetId) override;
	// ~ End of IEventflowEngineProvider

protected:

	UPROPERTY()
	UQuestStorageManager* StorageManager;

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> Quests;

	UPROPERTY()
	TWeakObjectPtr<UQuestEngine> FocusedQuest;


	UQuestEngine* GetQuestEngine(const FPrimaryAssetId& AssetId) const;
	void RemoveQuests();

	void HandleOnStorageLoaded(UObject* InManager);

	// ~ Binding
	virtual void HandleOnQuestStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId);
	// ~ End of Binding

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

private:

	UPROPERTY()
	TArray<TObjectPtr<UQuestEngine>> EnginePool;

public:

#if UE_BUILD_DEVELOPMENT
	RQUEST_API const TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>>& GetEditorQuests() const;
#endif

	static RQUEST_API UQuestSubsystem* Get(UWorld* World);

};


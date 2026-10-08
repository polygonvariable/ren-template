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
// class UQuestStorageManager;
class UQuestEngine;
class AQuestActor;


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestSubsystem : public UWorldSubsystem, public IEventflowEngineProvider
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartQuest(const FPrimaryAssetId& AssetId);

	UFUNCTION(BlueprintCallable)
	void CancelQuest(const FPrimaryAssetId& AssetId);


	// ~ IEventflowEngineProvider
	virtual void StartEventflow(const FPrimaryAssetId& AssetId) override;
	virtual void StopEventflow(const FPrimaryAssetId& AssetId) override;
	// ~ End of IEventflowEngineProvider

protected:

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> Quests;


	UQuestEngine* GetQuestEngine(const FPrimaryAssetId& AssetId) const;
	void RemoveQuests();

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


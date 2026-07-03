// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Definition/StateMachine.h"
#include "Subsystems/WorldSubsystem.h"

// Generated Headers
#include "QuestSubsystem.generated.h"

// Module Macros
#define REN_API RQUEST_API

// Forward Declarations
// class UQuestStorageManager;
class UQuestEngine;
class AQuestActor;


/**
 * 
 */
UCLASS(MinimalAPI)
class UQuestSubsystem : public UWorldSubsystem
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void StartQuest(const FPrimaryAssetId& AssetId);

	UFUNCTION(BlueprintCallable)
	void CancelQuest(const FPrimaryAssetId& AssetId);

protected:

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UQuestEngine>> ActiveQuests;


	virtual void HandleOnStateChanged(EFSMState PreviousState, EFSMState NewState, EFSMResult Result, FPrimaryAssetId AssetId);

	// ~ UWorldSubsystem
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldComponentsUpdated(UWorld& InWorld) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	// ~ End of UWorldSubsystem

public:

	static REN_API UQuestSubsystem* Get(UWorld* World);
	static REN_API UQuestSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API


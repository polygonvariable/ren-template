// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "RewardSubsystem.generated.h"

// Module Macros
#define REN_API RREWARD_API

// Forward Declarations
class IStorageProvider;
class UBroadcastSubsystem;
class UAuthActionSubsystem;
class URewardStorageManager;
struct FInstancedStruct;
struct FTaskResult;


/**
 *
 */
UCLASS(MinimalAPI)
class URewardSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	URewardStorageManager* GetStorageManager() const;

protected:

	IStorageProvider* StorageProvider = nullptr;
	TObjectPtr<UAuthActionSubsystem> AuthActionSubsystem = nullptr;
	TObjectPtr<UBroadcastSubsystem> BroadcastSubsystem = nullptr;


	void HandleRewardInvoke(const FInstancedStruct& Payload);
	void HandleStorageLoaded(const FTaskResult& Result);
	void HandleGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API URewardSubsystem* Get(UWorld* World);
	static REN_API URewardSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API


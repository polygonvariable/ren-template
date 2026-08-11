// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Generated Headers
#include "EnemySubsystem.generated.h"

// Module Macros
#define REN_API RENEMY_API

// Forward Declarations
class IStorageProvider;
class UEnemyStorageManager;


/**
 * 
 */
UCLASS(MinimalAPI)
class UEnemySubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UEnemyStorageManager* GetStorageManager();

protected:

	TObjectPtr<UEnemyStorageManager> StorageManager = nullptr;
	IStorageProvider* StorageProvider = nullptr;


	// ~ Binding
	void HandleOnPreGameInitialized();
	// ~ End of Binding

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	static REN_API UEnemySubsystem* Get(UWorld* World);
	static REN_API UEnemySubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API


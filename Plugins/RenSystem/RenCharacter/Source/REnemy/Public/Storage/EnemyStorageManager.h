// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "EnemyStorageManager.generated.h"

// Module Macros
#define REN_API RENEMY_API

// Forward Declarations
class UEnemyStorage;


/**
 *
 */
UCLASS(MinimalAPI)
class UEnemyStorageManager : public UObject, public IStorageManager
{

	GENERATED_BODY()

public:

	REN_API void AddKillTimestamp(const FGuid& EnemyId);
	REN_API void RemoveKillTimestamp(const FGuid& EnemyId);
	REN_API FDateTime GetKillTimestamp(const FGuid& EnemyId) const;
	REN_API bool ContainsKillTimestamp(const FGuid& EnemyId) const;

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UEnemyStorage> LocalStorage = nullptr;

	FGameEventDelegate OnStorageUpdated;

};


// Module Macros
#undef REN_API


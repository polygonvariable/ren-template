// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/StorageManager.h"

// Generated Headers
#include "QuestStorageManager.generated.h"

// Module Macros
#define REN_API RENEMY_API

// Forward Declarations
class UQuestStorage;


/**
 *
 */
UCLASS(MinimalAPI)
class UQuestStorageManager : public UObject, public IStorageManager
{

	GENERATED_BODY()

public:

	void AddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid);
	void UpdateActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid);
	void RemoveActiveQuest(const FPrimaryAssetId& QuestId);
	const TMap<FPrimaryAssetId, FGuid>* GetActiveQuests() const;

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UQuestStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API


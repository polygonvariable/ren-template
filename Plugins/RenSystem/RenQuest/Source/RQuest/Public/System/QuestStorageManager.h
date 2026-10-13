// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/StorageManager.h"

// Generated Headers
#include "QuestStorageManager.generated.h"


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

	FGuid GetActiveQuest(const FPrimaryAssetId& QuestId);

	void FindAddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid);
	bool AddActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid);
	bool CompleteActiveQuest(const FPrimaryAssetId& QuestId);

	void UpdateActiveQuest(const FPrimaryAssetId& QuestId, const FGuid& Guid);
	void RemoveActiveQuest(const FPrimaryAssetId& QuestId);


	bool ContainsActiveQuest(const FPrimaryAssetId& QuestId, bool& OutResult);
	bool ContainsCompletedQuest(const FPrimaryAssetId& QuestId, bool& OutResult);


	const TMap<FPrimaryAssetId, FGuid>* GetActiveQuests() const;

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UQuestStorage> LocalStorage = nullptr;

};


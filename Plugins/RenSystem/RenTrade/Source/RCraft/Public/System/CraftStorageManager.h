// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "CraftStorageManager.generated.h"

// Module Macros
#define REN_API RCRAFT_API

// Forward Declarations
class UCraftStorage;
struct FCraftInstance;
struct FTradeKey;


/**
 *
 */
UCLASS(MinimalAPI)
class UCraftStorageManager : public UObject, public IStorageManager
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;


	REN_API const FCraftInstance* GetItem(const FTradeKey& TradeKey) const;
	REN_API bool AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime);
	REN_API void ResetItems();
	REN_API int ClaimCraftedItems(const FTradeKey& TradeKey);

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UCraftStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API


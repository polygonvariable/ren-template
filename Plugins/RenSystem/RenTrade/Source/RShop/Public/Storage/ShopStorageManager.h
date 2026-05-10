// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "ShopStorageManager.generated.h"

// Module Macros
#define REN_API RSHOP_API

// Forward Declarations
class UShopStorage;
struct FShopData;
struct FTradeKey;


/**
 *
 */
UCLASS(MinimalAPI)
class UShopStorageManager : public UObject, public IStorageManager
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;


	REN_API const FShopData* GetItem(const FTradeKey& TradeKey) const;
	REN_API bool AddItem(const FTradeKey& TradeKey);
	REN_API void ResetItems();

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UShopStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API


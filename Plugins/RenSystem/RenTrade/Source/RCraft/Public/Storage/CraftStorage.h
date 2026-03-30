// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Definition/Runtime/TradeKey.h"
#include "SaveGame/Storage.h"
#include "Definition/CraftQuery.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "CraftStorage.generated.h"

// Module Macros
#define REN_API RCRAFT_API

// Forward Declarations
class UAssetCollection;
class UStorage;
class IStorageProvider;
class UCraftStorage;
class UTradeAsset;
class UCoreDataAsset;

struct FAssetDetail_Trade;
struct FInstancedStruct;
struct FCraftData;



/**
 *
 */
UCLASS(MinimalAPI)
class UCraftStorage : public UStorage
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnCraftUpdated);
	FOnCraftUpdated OnCraftUpdated;


	REN_API const FCraftData* GetItem(const FTradeKey& TradeKey) const;
	REN_API bool AddItem(const FTradeKey& TradeKey, FTimespan BatchProcessingTime);
	REN_API void ResetItems();

	REN_API int ClaimCraftedItems(const FTradeKey& TradeKey);

protected:

	UPROPERTY(SaveGame)
	TMap<FTradeKey, FCraftData> CraftItems;

};



// Module Macros
#undef REN_API


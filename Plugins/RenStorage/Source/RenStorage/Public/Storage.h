// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Project Headers
#include "RenCore/Public/Record/ClockRecord.h"
#include "RenCore/Public/Record/InventoryRecord.h"
#include "RenCore/Public/Inventory/InventoryInterface.h"
#include "RenCore/Public/Interface/ClockProviderInterface.h"
#include "RenCore/Public/Interface/WorldHistoryProviderInterface.h"

// Generated Headers
#include "Storage.generated.h"



/**
 *
 */
UCLASS()
class RENSTORAGE_API UStorage : public USaveGame, public IInventoryStorageInterface, public IClockRecordProviderInterface, public IWorldHistoryProviderInterface
{

	GENERATED_BODY()

public:
	
	/**
	 * A map of inventory items
	 * <FName = UUID, FInventoryRecord>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FInventoryRecord> InventoryRecords;

	/**
	 * A list of events that happened, which is a map of UUIDs to timestamps
	 * <FName = UUID, FDateTime>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FDateTime> WorldHistory;

	/**
	 * A map of clock related data according to level's name
	 * <FName = Level Name, FClockRecord>
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FClockRecord> ClockRecords;


	virtual TMap<FName, FInventoryRecord>& GetInventory() override { return InventoryRecords; }

	virtual const TMap<FName, FInventoryRecord>& GetInventoryRecords() const override { return InventoryRecords; }
	virtual TMap<FName, FInventoryRecord>& GetMutableInventoryRecords() override { return InventoryRecords; }

	virtual const TMap<FName, FClockRecord>& GetClockRecords() const override { return ClockRecords; }
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() override { return ClockRecords; }

	virtual const TMap<FName, FDateTime>& GetWorldHistory() const override { return WorldHistory; }
	virtual TMap<FName, FDateTime>& GetMutableWorldHistory() override { return WorldHistory; }

};


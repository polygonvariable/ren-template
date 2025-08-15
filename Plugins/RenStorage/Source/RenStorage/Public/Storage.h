// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Project Headers
#include "RenCore/Public/Interface/ClockProviderInterface.h"

#include "RenCoreInventory/Public/InventoryContainer.h"
#include "RenCoreInventory/Public/InventoryProviderInterface.h"

#include "RenCore/Public/Record/ClockRecord.h"
#include "RenCore/Public/EventTimestamp/EventTimestampProviderInterface.h"

// Generated Headers
#include "Storage.generated.h"



/**
 *
 */
UCLASS()
class UStorage : public USaveGame, public IInventoryProviderInterface, public IClockRecordProviderInterface, public IEventTimestampProviderInterface
{

	GENERATED_BODY()

public:
	
	/**
	 * A pair of inventory container with value
	 * TMap<FName = Container UUID, TMap<FName = Item UUID, FInventoryRecord>>
	 */
	UPROPERTY()
	TMap<FName, FInventoryContainer> InventoryContainers;

	/**
	 * A list of events that happened, which is a map of UUIDs to timestamps
	 * TMap<FName = UUID, FDateTime>
	 */
	UPROPERTY()
	TMap<FName, FDateTime> EventTimestamp;

	/**
	 * A map of clock related data according to level's name
	 * <FName = Level Name, FClockRecord>
	 */
	UPROPERTY()
	TMap<FName, FClockRecord> ClockRecords;

	virtual const TMap<FName, FInventoryContainer>& GetInventoryContainer() const override { return InventoryContainers; }
	virtual TMap<FName, FInventoryContainer>& GetMutableInventoryContainer() override { return InventoryContainers; }

	virtual const TMap<FName, FClockRecord>& GetClockRecords() const override { return ClockRecords; }
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() override { return ClockRecords; }

	virtual const TMap<FName, FDateTime>& GetEventTimestamp() const override { return EventTimestamp; }
	virtual TMap<FName, FDateTime>& GetMutableEventTimestamp() override { return EventTimestamp; }

};


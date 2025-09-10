// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

// Project Headers
#include "RCoreClock/Public/ClockProviderInterface.h"
#include "RCoreClock/Public/ClockRecord.h"

#include "RCoreInventory/Public/InventoryContainer.h"
#include "RCoreInventory/Public/InventoryProviderInterface.h"

#include "RCoreTimestamp/Public/TimestampProviderInterface.h"

#include "RCoreQuest/Public/QuestProviderInterface.h"
#include "RCoreQuest/Public/QuestRecord.h"


// Generated Headers
#include "Storage.generated.h"



/**
 *
 */
UCLASS()
class UStorage : public USaveGame, public IInventoryProviderInterface, public IClockProviderInterface, public ITimestampProviderInterface, public IQuestProviderInterface
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
	TMap<FName, FDateTime> Timestamps;

	/**
	 * A map of clock related data according to level's name
	 * <FName = Level Name, FClockRecord>
	 */
	UPROPERTY()
	TMap<FName, FClockRecord> ClockRecords;

	/**
	 * TMap<FGuid, FQuestRecord>
	 */
	UPROPERTY()
	TMap<FGuid, FQuestRecord> ActiveQuests;

	/**
	 * TMap<FGuid, FDateTime>
	 */
	UPROPERTY()
	TMap<FGuid, FDateTime> CompletedQuests;


	// ~ IInventoryProviderInterface
	virtual const TMap<FName, FInventoryContainer>& GetInventoryContainer() const override { return InventoryContainers; }
	virtual TMap<FName, FInventoryContainer>& GetMutableInventoryContainer() override { return InventoryContainers; }
	// ~ End of IInventoryProviderInterface

	// ~ IClockRecordProviderInterface
	virtual const TMap<FName, FClockRecord>& GetClockRecords() const override { return ClockRecords; }
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() override { return ClockRecords; }
	// ~ End of IClockRecordProviderInterface

	// ~ ITimestampProviderInterface
	virtual const TMap<FName, FDateTime>& GetTimestamp() const override { return Timestamps; }
	virtual TMap<FName, FDateTime>& GetMutableTimestamp() override { return Timestamps; }
	// ~ End of ITimestampProviderInterface

	// ~ IQuestProviderInterface
	virtual const TMap<FGuid, FQuestRecord>& GetActiveQuests() const override { return ActiveQuests; }
	virtual TMap<FGuid, FQuestRecord>& GetMutableActiveQuests() override { return ActiveQuests; }

	virtual const TMap<FGuid, FDateTime>& GetCompletedQuests() const override { return CompletedQuests; }
	virtual TMap<FGuid, FDateTime>& GetMutableCompletedQuests() override { return CompletedQuests; }
	// ~ End of IQuestProviderInterface

};


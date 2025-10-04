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

#include "RCoreCounter/Public/CounterContainer.h"
#include "RCoreCounter/Public/CounterProviderInterface.h"

#include "RCoreQuest/Public/QuestProviderInterface.h"
#include "RCoreQuest/Public/QuestRecord.h"

// Generated Headers
#include "Storage.generated.h"



/**
 *
 */
UCLASS()
class UStorage : public USaveGame,
	public IInventoryProviderInterface,
	public IClockProviderInterface,
	public IQuestProviderInterface,
	public ICounterProviderInterface
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TMap<FName, FCounterContainer> CounterContainers;

	/**
	 * A pair of inventory container with value
	 * TMap<FName = Container UUID, TMap<FName = Item UUID, FInventoryRecord>>
	 */
	UPROPERTY()
	TMap<FName, FInventoryContainer> InventoryContainers;

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
	TMap<FName, FQuestRecord> ActiveQuests;

	/**
	 * TMap<FGuid, FDateTime>
	 */
	UPROPERTY()
	TMap<FName, FDateTime> CompletedQuests;


	// ~ IInventoryProviderInterface
	virtual TMap<FName, FCounterContainer>& GetCounterContainer() override { return CounterContainers; }
	virtual const TMap<FName, FCounterContainer>& GetCounterContainer() const override { return CounterContainers; }
	// ~ End of IInventoryProviderInterface
	// 
	// ~ IInventoryProviderInterface
	virtual const TMap<FName, FInventoryContainer>& GetInventoryContainer() const override { return InventoryContainers; }
	virtual TMap<FName, FInventoryContainer>& GetMutableInventoryContainer() override { return InventoryContainers; }
	// ~ End of IInventoryProviderInterface

	// ~ IClockRecordProviderInterface
	virtual const TMap<FName, FClockRecord>& GetClockRecords() const override { return ClockRecords; }
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() override { return ClockRecords; }
	// ~ End of IClockRecordProviderInterface

	// ~ IQuestProviderInterface
	virtual const TMap<FName, FQuestRecord>& GetActiveQuests() const override { return ActiveQuests; }
	virtual TMap<FName, FQuestRecord>& GetMutableActiveQuests() override { return ActiveQuests; }

	virtual const TMap<FName, FDateTime>& GetCompletedQuests() const override { return CompletedQuests; }
	virtual TMap<FName, FDateTime>& GetMutableCompletedQuests() override { return CompletedQuests; }
	// ~ End of IQuestProviderInterface

};


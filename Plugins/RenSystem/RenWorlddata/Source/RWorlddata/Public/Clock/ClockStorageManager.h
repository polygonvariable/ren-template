// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Clock/ClockInstance.h"
#include "Core/StorageManager.h"

// Generated Headers
#include "ClockStorageManager.generated.h"

// Forward Declarations
class UClockStorage;


/**
 *
 */
UCLASS(MinimalAPI)
class UClockStorageManager : public UObject, public IStorageManager
{

	GENERATED_BODY()

public:

	FClockInstance GetClockInstance(FName WorldId);
	void SetClockInstance(FName WorldId, FClockInstance ClockInstance);

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UClockStorage> LocalStorage = nullptr;

};


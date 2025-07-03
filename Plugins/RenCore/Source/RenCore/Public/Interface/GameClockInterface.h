// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers
#include "RenCore/Public/Record/ClockRecord.h"

// Generated Headers
#include "GameClockInterface.generated.h"

// Forward Declarations



UINTERFACE(MinimalAPI)
class UGameClockStorageInterface : public UInterface
{
	GENERATED_BODY()
};

class RENCORE_API IGameClockStorageInterface
{
	GENERATED_BODY()

public:

	virtual const TMap<FName, FClockRecord>& GetClockRecords() const = 0;
	virtual TMap<FName, FClockRecord>& GetMutableClockRecords() = 0;
	

};





DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameDayChanged, int, Day);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, float, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameClockStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameClockStopped);


UINTERFACE(MinimalAPI)
class UGameClockSubsystemInterface : public UInterface
{
	GENERATED_BODY()
};

class RENCORE_API IGameClockSubsystemInterface
{
	GENERATED_BODY()

public:

	virtual FOnGameDayChanged& GetOnGameDayChanged() = 0;
	virtual FOnGameTimeChanged& GetOnGameTimeChanged() = 0;
	virtual FOnGameClockStarted& GetOnGameClockStarted() = 0;
	virtual FOnGameClockStopped& GetOnGameClockStopped() = 0;

	virtual float GetSmoothNormalizedTime() const = 0;
	virtual bool GetIsActive() const = 0;

};


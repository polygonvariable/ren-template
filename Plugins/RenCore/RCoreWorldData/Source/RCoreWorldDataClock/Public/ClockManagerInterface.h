// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "ClockManagerInterface.generated.h"

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClockTimeChanged, int /* Time */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClockDayChanged, int /* Day */);


UINTERFACE(MinimalAPI)
class UClockManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RCOREWORLDDATACLOCK_API IClockManagerInterface
{

	GENERATED_BODY()

public:

	virtual float GetNormalizedTime() const = 0;
	virtual bool IsClockActive() const = 0;
	virtual bool GetDayLength(int& Length) const = 0;
	virtual bool GetYearLength(int& Length) const = 0;

	virtual int GetCurrentTime() const = 0;
	virtual int GetCurrentDay() const = 0;
	virtual int GetCurrentYear() const = 0;

	FOnClockTimeChanged& OnClockTimeChanged() { return ClockTimeChanged; };
	FOnClockDayChanged& OnClockDayChanged() { return ClockDayChanged; };

protected:

    FOnClockTimeChanged ClockTimeChanged;
    FOnClockDayChanged ClockDayChanged;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers

// Generated Headers

// Forward Declarations



/**
 * 
 */
struct FClockDelegates
{

public:

    DECLARE_MULTICAST_DELEGATE_OneParam(FOnDayChanged, int /* Day */);
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnYearChanged, int /* Year */);
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float /* Time */);
    DECLARE_MULTICAST_DELEGATE(FOnClockStarted);
    DECLARE_MULTICAST_DELEGATE(FOnClockStopped);

    FOnDayChanged OnDayChanged;
    FOnYearChanged OnYearChanged;
    FOnTimeChanged OnTimeChanged;
    FOnClockStarted OnClockStarted;
    FOnClockStopped OnClockStopped;

};


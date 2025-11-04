// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Forward Declarations

// Delegate Declarations
DECLARE_MULTICAST_DELEGATE(FOnWeatherRefreshed);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, UPrimaryDataAsset* /* WeatherAsset */);



/**
 *
 */
struct FWeatherDelegates
{

public:

    FOnWeatherRefreshed OnRefreshed;
    FOnWeatherChanged OnChanged;
    FOnWeatherChanged OnRemoved;

    void RemoveAll(UObject* Target)
    {
        OnRefreshed.RemoveAll(Target);
        OnChanged.RemoveAll(Target);
        OnRemoved.RemoveAll(Target);
    }

    void Clear()
    {
		OnRefreshed.Clear();
		OnChanged.Clear();
		OnRemoved.Clear();
    }

};


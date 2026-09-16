// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Clock/ClockWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR

#endif


#if WITH_EDITOR
EDataValidationResult UClockWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {

    }

    return Result;
}
#endif


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Clock/ClockWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "UObject/ObjectSaveContext.h"
#endif

// Project Headers
#if WITH_EDITOR

#endif


#if WITH_EDITOR
void UClockWorldConfig::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
    Super::PreSave(ObjectSaveContext);
    
    DefaultClock.Clamp(DayLength, YearLength);
}

EDataValidationResult UClockWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {
        if (TickInterval < 0.05f || TickInterval > 10.0f)
        {
            Context.AddError(FText::FromString("Invalid range of tick interval"));
            return EDataValidationResult::Invalid;
        }
        if (DayLength < 1)
        {
            Context.AddError(FText::FromString("Day length should be greather than 0"));
            return EDataValidationResult::Invalid;
        }
        if (YearLength < 1)
        {
            Context.AddError(FText::FromString("Year length should be greather than 0"));
            return EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif


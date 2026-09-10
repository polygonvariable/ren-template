// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/WeatherWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Materials/MaterialParameterCollection.h"

// Project Headers
#include "Actor/WeatherEffectManagerActor.h"
#include "System/WeatherController.h"


#if WITH_EDITORONLY_DATA
EDataValidationResult UWeatherWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {
        if (!IsValid(WeatherMPC))
        {
            Context.AddError(FText::FromString("Weather material parameter is invalid"));
            Result = EDataValidationResult::Invalid;
        }
        else if (!IsValid(WeatherController))
        {
            Context.AddError(FText::FromString("Weather controller is invalid"));
            Result = EDataValidationResult::Invalid;
        }
        else if (!IsValid(EffectManager))
        {
            Context.AddError(FText::FromString("Weather effect manager is invalid"));
            Result = EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif


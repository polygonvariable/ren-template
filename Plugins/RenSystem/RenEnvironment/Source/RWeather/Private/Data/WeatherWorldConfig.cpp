// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/WeatherWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Materials/MaterialParameterCollection.h"
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "Actor/WeatherEffectManagerActor.h"
#include "System/WeatherController.h"
#endif


#if WITH_EDITOR
EDataValidationResult UWeatherWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {
        if (!DefaultWeather.IsValid())
        {
            Context.AddError(FText::FromString("Weather asset is invalid"));
            return EDataValidationResult::Invalid;
        }

        if (DefaultPriority <= 0)
        {
            Context.AddError(FText::FromString("Invalid weather priority"));
            return EDataValidationResult::Invalid;
        }

        if (!IsValid(WeatherMPC))
        {
            Context.AddError(FText::FromString("Weather material parameter is invalid"));
            return EDataValidationResult::Invalid;
        }
        
        if (!IsValid(WeatherController))
        {
            Context.AddError(FText::FromString("Weather controller is invalid"));
            return EDataValidationResult::Invalid;
        }
        
        if (!IsValid(EffectManager))
        {
            Context.AddError(FText::FromString("Weather effect manager is invalid"));
            return EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif


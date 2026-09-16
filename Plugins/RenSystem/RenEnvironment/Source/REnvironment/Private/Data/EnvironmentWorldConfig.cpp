// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EnvironmentWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "System/EnvironmentController.h"
#endif


#if WITH_EDITOR
EDataValidationResult UEnvironmentWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);
    if (bEnabled)
    {
        for (TSubclassOf<UEnvironmentStackedController> Controller : StackedControllers)
        {
            if (!IsValid(Controller))
            {
                Context.AddError(FText::FromString("Environment stacked controller is invalid"));
                return EDataValidationResult::Invalid;
            }
        }

        for (TSubclassOf<UEnvironmentDiscreteController> Controller : DiscreteControllers)
        {
            if (!IsValid(Controller))
            {
                Context.AddError(FText::FromString("Environment stacked controller is invalid"));
                return EDataValidationResult::Invalid;
            }
        }

        for (const FPrimaryAssetId& Profile : DefaultProfiles)
        {
            if (!Profile.IsValid())
            {
                Context.AddError(FText::FromString("Environment profile asset is invalid"));
                return EDataValidationResult::Invalid;
            }
        }

        if (ProfilePriority <= 0)
        {
            Context.AddError(FText::FromString("Invalid environment profile priority"));
            return EDataValidationResult::Invalid;
        }
    }
    return Result;
}
#endif


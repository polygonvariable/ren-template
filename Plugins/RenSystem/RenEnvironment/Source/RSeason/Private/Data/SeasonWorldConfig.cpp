// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/SeasonWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Materials/MaterialParameterCollection.h"
#include "Misc/DataValidation.h"
#endif

// Project Headers
#if WITH_EDITOR
#include "System/SeasonController.h"
#endif
#include "WorldFragmentSettings.h"


#if WITH_EDITOR
EDataValidationResult USeasonWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {
        if (!DefaultSeason.IsValid())
        {
            Context.AddError(FText::FromString("Season asset is invalid"));
            return EDataValidationResult::Invalid;
        }

        if (DefaultPriority <= 0)
        {
            Context.AddError(FText::FromString("Invalid season priority"));
            return EDataValidationResult::Invalid;
        }

        if (!IsValid(SeasonMPC))
        {
            Context.AddError(FText::FromString("Season material parameter is invalid"));
            return EDataValidationResult::Invalid;
        }

        if (!IsValid(SeasonController))
        {
            Context.AddError(FText::FromString("Season controller is invalid"));
            return EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif

const USeasonWorldConfig* USeasonWorldConfig::Get(UWorld* World)
{
    AWorldFragmentSettings* WorldSettings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
    if (!IsValid(WorldSettings))
    {
        return nullptr;
    }
    return WorldSettings->FindConfigByClass<USeasonWorldConfig>();
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EnvironmentPaintWorldConfig.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


#if WITH_EDITORONLY_DATA
EDataValidationResult UEnvironmentPaintWorldConfig::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (bEnabled)
    {
        if (NiagaraSystem.IsNull())
        {
            Context.AddError(FText::FromString("Niagara system is invalid"));
            Result = EDataValidationResult::Invalid;
        }
        else if (MainRT.IsNull())
        {
            Context.AddError(FText::FromString("Main render target is invalid"));
            Result = EDataValidationResult::Invalid;
        }
        else if (PersistentRT.IsNull())
        {
            Context.AddError(FText::FromString("Presistent render target is invalid"));
            Result = EDataValidationResult::Invalid;
        }
        else if (MPC.IsNull())
        {
            Context.AddError(FText::FromString("Canvas aterial parameter is invalid"));
            Result = EDataValidationResult::Invalid;
        }
    }

    return Result;
}
#endif


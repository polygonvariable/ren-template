// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/WeatherAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Data/EnvironmentProfileAsset.h"
#if WITH_EDITOR
#include "Core/WeatherSettings.h"
#include "Data/WeatherWorldConfig.h"
#include "MaterialLibrary.h"
#include "WorldFragmentSettings.h"
#endif


FPrimaryAssetId UWeatherAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
}

#if WITH_EDITOR
void UWeatherAsset::ApplySurfaceToWorld()
{
    if (!GEditor)
    {
        return;
    }

    UWorld* World = GEditor->GetEditorWorldContext().World();
    if (IsValid(World))
    {
        AWorldFragmentSettings* WorldSettings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
        if (!IsValid(WorldSettings))
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to get world settings")));
            return;
        }

        const UWeatherSettings* Settings = UWeatherSettings::Get();
        const UWeatherWorldConfig* WorldConfig = WorldSettings->FindConfigByClass<UWeatherWorldConfig>();
        if (!IsValid(WorldConfig))
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Unable to get weather world config")));
            return;
        }

        UMaterialParameterCollectionInstance* MPCInstance = World->GetParameterCollectionInstance(WorldConfig->WeatherMPC);
        if (!MPCInstance)
        {
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Unable to get weather MPC")));
            return;
        }

        FMaterialLibrary::SetSurfaceProperty(SurfaceProperty, MPCInstance, Settings->SurfaceTint, Settings->SurfaceSROW, Settings->SurfaceDCMA);
    }
}

EDataValidationResult UWeatherAsset::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    for (const TSoftObjectPtr<UNiagaraSystem>& Item : NiagaraSystems)
    {
        if (Item.IsNull())
        {
            Context.AddError(FText::FromString("Invalid niagara system in list"));
            return EDataValidationResult::Invalid;
        }
    }

    for (UEnvironmentProfileAsset* Item : EnvironmentProfiles)
    {
        if (!IsValid(Item))
        {
            Context.AddError(FText::FromString("Empty environment profile field in list"));
            return EDataValidationResult::Invalid;
        }

        EDataValidationResult ItemResult = Item->IsDataValid(Context);
        if (ItemResult == EDataValidationResult::Invalid)
        {
            Context.AddError(FText::FromString("Invalid environment profile in list"));
            return EDataValidationResult::Invalid;
        }
    }

    if (ProfilePriority <= 0)
    {
        Context.AddError(FText::FromString("Invalid weather priority"));
        return EDataValidationResult::Invalid;
    }

    if (TransitionRate < 0.05 || TransitionDuration < 0.5f || TransitionDuration > 30.0f)
    {
        Context.AddError(FText::FromString("Invalid transition duration or rate"));
        return EDataValidationResult::Invalid;
    }

    if (!IsValid(TransitionCurve))
    {
        Context.AddError(FText::FromString("Invalid transition curve"));
        return EDataValidationResult::Invalid;
    }

    return Result;
}
#endif

FPrimaryAssetType UWeatherAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Environment.Weather"));
}


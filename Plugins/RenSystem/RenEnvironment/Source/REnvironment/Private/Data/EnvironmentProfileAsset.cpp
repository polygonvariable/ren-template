// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EnvironmentProfileAsset.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif


FPrimaryAssetId UEnvironmentProfileAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UEnvironmentProfileAsset::GetPrimaryAssetType(), GetFName());
}

#if WITH_EDITORONLY_DATA
EDataValidationResult UEnvironmentProfileAsset::IsDataValid(FDataValidationContext& Context) const
{
    EDataValidationResult Result = Super::IsDataValid(Context);

    if (ProfileType == EEnvironmentProfileType::Default)
    {
        Context.AddError(FText::FromString("Invalid environment profile type"));
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

FPrimaryAssetType UEnvironmentProfileAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Environment.Profile"));
}


UEnvironmentFogProfileAsset::UEnvironmentFogProfileAsset()
{
    ProfileType = EEnvironmentProfileType::Fog;
}

UEnvironmentLightProfileAsset::UEnvironmentLightProfileAsset()
{
    ProfileType = EEnvironmentProfileType::Light;
}

UEnvironmentAtmosphereProfileAsset::UEnvironmentAtmosphereProfileAsset()
{
    ProfileType = EEnvironmentProfileType::Atmosphere;
}


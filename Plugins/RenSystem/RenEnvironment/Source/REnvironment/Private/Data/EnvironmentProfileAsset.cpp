// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EnvironmentProfileAsset.h"


FPrimaryAssetId UEnvironmentProfileAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(UEnvironmentProfileAsset::GetPrimaryAssetType(), GetFName());
}

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


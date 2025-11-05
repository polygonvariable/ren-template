// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentEdAction.h"

// Engine Headers

// Project Headers
#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"



FText FEnvironmentProfileActions::GetName() const
{
    return FText::FromString(TEXT("Environment Profile"));
}

FColor FEnvironmentProfileActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#3D7C31"));
}

UClass* FEnvironmentProfileActions::GetSupportedClass() const
{
    return UEnvironmentProfileAsset::StaticClass();
}

uint32 FEnvironmentProfileActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}


FText FEnvironmentFogProfileActions::GetName() const
{
    return FText::FromString(TEXT("Fog Profile"));
}

FColor FEnvironmentFogProfileActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#3D7C31"));
}

UClass* FEnvironmentFogProfileActions::GetSupportedClass() const
{
    return UEnvironmentFogProfileAsset::StaticClass();
}

uint32 FEnvironmentFogProfileActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}



FText FEnvironmentLightProfileActions::GetName() const
{
    return FText::FromString(TEXT("Light Profile"));
}

FColor FEnvironmentLightProfileActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#3D7C31"));
}

UClass* FEnvironmentLightProfileActions::GetSupportedClass() const
{
    return UEnvironmentLightProfileAsset::StaticClass();
}

uint32 FEnvironmentLightProfileActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}



FText FEnvironmentAtmosphereProfileActions::GetName() const
{
    return FText::FromString(TEXT("Atmosphere Profile"));
}

FColor FEnvironmentAtmosphereProfileActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#3D7C31"));
}

UClass* FEnvironmentAtmosphereProfileActions::GetSupportedClass() const
{
    return UEnvironmentAtmosphereProfileAsset::StaticClass();
}

uint32 FEnvironmentAtmosphereProfileActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}



FText FEnvironmentAssetActions::GetName() const
{
    return FText::FromString(TEXT("Environment Asset"));
}

FColor FEnvironmentAssetActions::GetTypeColor() const
{
    return FColor::FromHex(TEXT("#3D7C31"));
}

UClass* FEnvironmentAssetActions::GetSupportedClass() const
{
    return UEnvironmentAsset::StaticClass();
}

uint32 FEnvironmentAssetActions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}


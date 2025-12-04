// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentEdAction.h"

// Engine Headers

// Project Headers
#include "RenEnvironment/Public/Asset/EnvironmentAsset.h"
#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"



#define ACTION_METHODS_IMPLEMENTATION(Class, Name, Color, Type, Category) \
    FText Class::GetName() const \
    { \
        return FText::FromString(Name); \
    } \
    FColor Class::GetTypeColor() const \
    { \
        return FColor::FromHex(Color); \
    } \
    UClass* Class::GetSupportedClass() const \
    { \
        return Type::StaticClass(); \
    } \
    uint32 Class::GetCategories() \
    { \
        return Category; \
    }

ACTION_METHODS_IMPLEMENTATION(FEnvironmentProfileActions, TEXT("Environment Profile"), TEXT("#3D7C31"), UEnvironmentProfileAsset, EAssetTypeCategories::Misc)

ACTION_METHODS_IMPLEMENTATION(FEnvironmentFogProfileActions, TEXT("Fog Profile"), TEXT("#3D7C31"), UEnvironmentFogProfileAsset, EAssetTypeCategories::Misc)
ACTION_METHODS_IMPLEMENTATION(FEnvironmentLightProfileActions, TEXT("Light Profile"), TEXT("#3D7C31"), UEnvironmentLightProfileAsset, EAssetTypeCategories::Misc)
ACTION_METHODS_IMPLEMENTATION(FEnvironmentAtmosphereProfileActions, TEXT("Atmosphere Profile"), TEXT("#3D7C31"), UEnvironmentAtmosphereProfileAsset, EAssetTypeCategories::Misc)

ACTION_METHODS_IMPLEMENTATION(FEnvironmentAssetActions, TEXT("Environment Asset"), TEXT("#3D7C31"), UEnvironmentAsset, EAssetTypeCategories::Misc)


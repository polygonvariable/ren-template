// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnvironmentEdAction.h"

// Engine Headers

// Project Headers
#include "Data/EnvironmentAsset.h"
#include "Data/EnvironmentProfileAsset.h"


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

ACTION_METHODS_IMPLEMENTATION(FEnvironmentFogProfileActions, TEXT("Fog Profile"), TEXT("#3D7C31"), UEnvironmentFogProfileAsset, EAssetTypeCategories::Misc)
ACTION_METHODS_IMPLEMENTATION(FEnvironmentLightProfileActions, TEXT("Light Profile"), TEXT("#3D7C31"), UEnvironmentLightProfileAsset, EAssetTypeCategories::Misc)
ACTION_METHODS_IMPLEMENTATION(FEnvironmentAtmosphereProfileActions, TEXT("Atmosphere Profile"), TEXT("#3D7C31"), UEnvironmentAtmosphereProfileAsset, EAssetTypeCategories::Misc)

ACTION_METHODS_IMPLEMENTATION(FEnvironmentWorldConfigActions, TEXT("Environment World Config"), TEXT("#3D7C31"), UEnvironmentWorldConfig, EAssetTypeCategories::Misc)


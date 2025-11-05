// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

// Project Headers
#include "RenEnvironment/Public/Profile/EnvironmentProfileType.h"

// Generated Headers
#include "EnvironmentProfileAsset.generated.h"



/**
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UEnvironmentProfileAsset : public UPrimaryDataAsset
{

    GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, AssetRegistrySearchable)
    EEnvironmentProfileType ProfileType;

public:

    // ~ UPrimaryDataAsset
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
    }
    // ~ End of UPrimaryDataAsset



    static FPrimaryAssetType GetPrimaryAssetType()
    {
        return FPrimaryAssetType(TEXT("Environment.Profile"));
    }

    static FPrimaryAssetId MakePrimaryAssetId(const FName& AssetName)
    {
        return FPrimaryAssetId(GetPrimaryAssetType(), AssetName);
    }

    static bool IsValid(const FPrimaryAssetId& AssetId)
    {
        return AssetId.PrimaryAssetType == GetPrimaryAssetType();
    }

    static bool GetType(const FAssetData& AssetData, FName& ProfileType)
    {
        return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UEnvironmentProfileAsset, ProfileType), ProfileType);
    }

    static bool GetType(const FAssetData& AssetData, EEnvironmentProfileType& ProfileType)
    {
        if (!AssetData.IsValid())
        {
            return false;
        }

        FName TypeText;
        GetType(AssetData, TypeText);

        const UEnum* Enum = StaticEnum<EEnvironmentProfileType>();
        int64 EnumValue = Enum->GetValueByName(TypeText);
        if (EnumValue == INDEX_NONE)
        {
            ProfileType = EEnvironmentProfileType::Default;
            return false;
        }

        ProfileType = static_cast<EEnvironmentProfileType>(EnumValue);
        return true;
    }

};



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentFogProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentFogProfileAsset()
    {
        ProfileType = EEnvironmentProfileType::Fog;
    }

    UPROPERTY(EditDefaultsOnly)
    float FogDensity = 0.05f;
 
};



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentLightProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentLightProfileAsset()
    {
        ProfileType = EEnvironmentProfileType::Light;
    }

    UPROPERTY(EditDefaultsOnly)
    float SunIntensity = 20.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor SunColor = FColor::White;

    UPROPERTY(EditDefaultsOnly)
    float MoonIntensity = 5.0f;

    UPROPERTY(EditDefaultsOnly)
    FColor MoonColor = FColor::FromHex("#4B6F91");
 
};



/**
 * 
 */
UCLASS(MinimalAPI)
class UEnvironmentAtmosphereProfileAsset : public UEnvironmentProfileAsset
{
 
    GENERATED_BODY()
 
public:

    UEnvironmentAtmosphereProfileAsset()
    {
        ProfileType = EEnvironmentProfileType::Atmosphere;
    }

    UPROPERTY(EditDefaultsOnly)
    float MieScatteringScale = 0.003996f;

};


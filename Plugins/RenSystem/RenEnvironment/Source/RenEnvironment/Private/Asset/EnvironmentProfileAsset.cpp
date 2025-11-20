// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/EnvironmentProfileAsset.h"

// Engine Headers

// Project Header



FPrimaryAssetId UEnvironmentProfileAsset::GetPrimaryAssetId() const
{
    return FPrimaryAssetId(GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UEnvironmentProfileAsset::GetPrimaryAssetType()
{
    return FPrimaryAssetType(TEXT("Environment.Profile"));
}

FPrimaryAssetId UEnvironmentProfileAsset::MakePrimaryAssetId(const FName& AssetName)
{
    return FPrimaryAssetId(GetPrimaryAssetType(), AssetName);
}

bool UEnvironmentProfileAsset::IsValid(const FPrimaryAssetId& AssetId)
{
    bool bAssetValid = AssetId.IsValid();
    bool bTypeValid = AssetId.PrimaryAssetType == GetPrimaryAssetType();

    return bAssetValid && bTypeValid;
}

bool UEnvironmentProfileAsset::GetType(const FAssetData& AssetData, FName& ProfileType)
{
    return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UEnvironmentProfileAsset, ProfileType), ProfileType);
}

bool UEnvironmentProfileAsset::GetType(const FAssetData& AssetData, EEnvironmentProfileType& ProfileType)
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


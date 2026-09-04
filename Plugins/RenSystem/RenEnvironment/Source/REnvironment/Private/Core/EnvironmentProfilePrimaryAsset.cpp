// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EnvironmentProfilePrimaryAsset.h"

// Project Headers
#include "Data/EnvironmentProfileAsset.h"


FPrimaryAssetType FEnvironmentProfilePrimaryAsset::GetAssetType()
{
	return UEnvironmentProfileAsset::GetPrimaryAssetType();
}

FPrimaryAssetId FEnvironmentProfilePrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(FEnvironmentProfilePrimaryAsset::GetAssetType(), AssetName);
}

bool FEnvironmentProfilePrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == FEnvironmentProfilePrimaryAsset::GetAssetType();
}

bool FEnvironmentProfilePrimaryAsset::GetType(const FAssetData& AssetData, FName& ProfileType)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UEnvironmentProfileAsset, ProfileType), ProfileType);
}

bool FEnvironmentProfilePrimaryAsset::GetType(const FAssetData& AssetData, EEnvironmentProfileType& ProfileType)
{
    if (!AssetData.IsValid())
    {
        return false;
    }

    FName TypeText;
    FEnvironmentProfilePrimaryAsset::GetType(AssetData, TypeText);

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

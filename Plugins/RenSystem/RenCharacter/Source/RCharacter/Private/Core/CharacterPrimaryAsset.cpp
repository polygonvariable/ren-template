// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/CharacterPrimaryAsset.h"

// Project Headers
#include "Data/CharacterAsset.h"


FPrimaryAssetType FCharacterPrimaryAsset::GetAssetType()
{
	return UCharacterAsset::GetPrimaryAssetType();
}

FPrimaryAssetId FCharacterPrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(FCharacterPrimaryAsset::GetAssetType(), AssetName);
}

bool FCharacterPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == FCharacterPrimaryAsset::GetAssetType();
}

bool FCharacterPrimaryAsset::GetDisplayName(const FAssetData& AssetData, FText& DisplayName)
{
	if (!AssetData.IsValid())
	{
		return false;
	}
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UCharacterAsset, DisplayName), DisplayName);
}

bool FCharacterPrimaryAsset::GetHealth(const FAssetData& AssetData, int& Health)
{
	if (!AssetData.IsValid())
	{
		return false;
	}
	return AssetData.GetTagValue<int>(GET_MEMBER_NAME_CHECKED(UCharacterAsset, Health), Health);
}


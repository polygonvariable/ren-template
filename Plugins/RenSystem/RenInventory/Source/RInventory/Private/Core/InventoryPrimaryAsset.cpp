// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/InventoryPrimaryAsset.h"

// Project Headers
#include "Data/InventoryAsset.h"


FPrimaryAssetType FInventoryPrimaryAsset::GetAssetType()
{
	return UInventoryAsset::GetPrimaryAssetType();
}

FPrimaryAssetId FInventoryPrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(UInventoryAsset::GetPrimaryAssetType(), AssetName);
}

bool FInventoryPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == UInventoryAsset::GetPrimaryAssetType();
}

bool FInventoryPrimaryAsset::GetType(const FAssetData& AssetData, FName& Type)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, Type), Type);
}

bool FInventoryPrimaryAsset::GetType(const FAssetData& AssetData, EInventoryItemType& Type)
{
	FName TypeText;
	GetType(AssetData, TypeText);

	const UEnum* Enum = StaticEnum<EInventoryItemType>();
	int64 EnumValue = Enum->GetValueByName(TypeText);
	if (EnumValue == INDEX_NONE)
	{
		Type = EInventoryItemType::Default;
		return false;
	}

	Type = static_cast<EInventoryItemType>(EnumValue);
	return true;
}

bool FInventoryPrimaryAsset::GetRarity(const FAssetData& AssetData, FName& Rarity)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, Rarity), Rarity);
}

bool FInventoryPrimaryAsset::GetRarity(const FAssetData& AssetData, EInventoryItemRarity& Rarity)
{
	FName RarityText;
	GetRarity(AssetData, RarityText);

	const UEnum* Enum = StaticEnum<EInventoryItemType>();
	int64 EnumValue = Enum->GetValueByName(RarityText);
	if (EnumValue == INDEX_NONE)
	{
		Rarity = EInventoryItemRarity::Default;
		return false;
	}

	Rarity = static_cast<EInventoryItemRarity>(EnumValue);
	return true;
}

bool FInventoryPrimaryAsset::GetDisplayName(const FAssetData& AssetData, FText& DisplayName)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, DisplayName), DisplayName);
}

bool FInventoryPrimaryAsset::GetStackable(const FAssetData& AssetData, bool& bStackable)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bStackable), bStackable);
}

bool FInventoryPrimaryAsset::GetPersistWhenEmpty(const FAssetData& AssetData, bool& bPersistWhenEmpty)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bPersistWhenEmpty), bPersistWhenEmpty);
}


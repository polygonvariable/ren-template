// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "InventoryPrimaryAsset.h"

// Engine Headers

// Project Headers
#include "RCoreInventory/Public/InventoryAsset.h"



FPrimaryAssetType InventoryPrimaryAsset::GetAssetType()
{
	return FPrimaryAssetType(TEXT("Inventory"));
}

FPrimaryAssetId InventoryPrimaryAsset::GetPrimaryAssetId(const FName& ItemId)
{
	return FPrimaryAssetId(InventoryPrimaryAsset::GetAssetType(), ItemId);
}

bool InventoryPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == InventoryPrimaryAsset::GetAssetType();
}

bool InventoryPrimaryAsset::GetItemType(const FAssetData& AssetData, EInventoryItemType& ItemType)
{
	FName TypeText;
	AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, ItemType), TypeText);

	const UEnum* Enum = StaticEnum<EInventoryItemType>();
	int64 EnumValue = Enum->GetValueByName(TypeText);
	if (EnumValue == INDEX_NONE)
	{
		ItemType = EInventoryItemType::Default;
		return false;
	}

	ItemType = static_cast<EInventoryItemType>(EnumValue);
	return true;
}

bool InventoryPrimaryAsset::GetItemRarity(const FAssetData& AssetData, EInventoryItemRarity& ItemRarity)
{
	FName RarityText;
	AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, ItemRarity), RarityText);

	const UEnum* Enum = StaticEnum<EInventoryItemType>();
	int64 EnumValue = Enum->GetValueByName(RarityText);
	if (EnumValue == INDEX_NONE)
	{
		ItemRarity = EInventoryItemRarity::Default;
		return false;
	}

	ItemRarity = static_cast<EInventoryItemRarity>(EnumValue);
	return true;
}

bool InventoryPrimaryAsset::GetItemName(const FAssetData& AssetData, FText& ItemName)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, ItemName), ItemName);
}

bool InventoryPrimaryAsset::GetItemIsStackable(const FAssetData& AssetData, bool& bIsStackable)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bIsStackable), bIsStackable);
}

bool InventoryPrimaryAsset::GetItemAllowEmptyData(const FAssetData& AssetData, bool& bOutAllowEmptyData)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bAllowEmptyData), bOutAllowEmptyData);
}



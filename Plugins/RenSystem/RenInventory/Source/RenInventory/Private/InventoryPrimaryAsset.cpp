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

FPrimaryAssetId InventoryPrimaryAsset::GetPrimaryAssetId(const FName& AssetName)
{
	return FPrimaryAssetId(InventoryPrimaryAsset::GetAssetType(), AssetName);
}

bool InventoryPrimaryAsset::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == InventoryPrimaryAsset::GetAssetType();
}

bool InventoryPrimaryAsset::GetItemType(const FAssetData& AssetData, FName& ItemType)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, ItemType), ItemType);
}

bool InventoryPrimaryAsset::GetItemType(const FAssetData& AssetData, EInventoryItemType& ItemType)
{
	FName TypeText;
	GetItemType(AssetData, TypeText);

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

bool InventoryPrimaryAsset::GetItemRarity(const FAssetData& AssetData, FName& ItemRarity)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, ItemRarity), ItemRarity);
}

bool InventoryPrimaryAsset::GetItemRarity(const FAssetData& AssetData, EInventoryItemRarity& ItemRarity)
{
	FName RarityText;
	GetItemRarity(AssetData, RarityText);

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



const FName InventoryFilterProperty::AssetId(TEXT("AssetId"));
const FName InventoryFilterProperty::AssetType(TEXT("AssetType"));
const FName InventoryFilterProperty::AssetRarity(TEXT("AssetRarity"));

const FName InventoryFilterProperty::ItemId(TEXT("ItemId"));
const FName InventoryFilterProperty::ItemQuantity(TEXT("ItemQuantity"));
const FName InventoryFilterProperty::ItemExperience(TEXT("ItemExperience"));
const FName InventoryFilterProperty::ItemLevel(TEXT("ItemLevel"));
const FName InventoryFilterProperty::ItemRank(TEXT("ItemRank"));


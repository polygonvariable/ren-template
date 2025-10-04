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

bool InventoryPrimaryAsset::GetType(const FAssetData& AssetData, FName& Type)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, Type), Type);
}

bool InventoryPrimaryAsset::GetType(const FAssetData& AssetData, EInventoryItemType& Type)
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

bool InventoryPrimaryAsset::GetRarity(const FAssetData& AssetData, FName& Rarity)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, Rarity), Rarity);
}

bool InventoryPrimaryAsset::GetRarity(const FAssetData& AssetData, EInventoryItemRarity& Rarity)
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

bool InventoryPrimaryAsset::GetDisplayName(const FAssetData& AssetData, FText& DisplayName)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, DisplayName), DisplayName);
}

bool InventoryPrimaryAsset::GetStackable(const FAssetData& AssetData, bool& bStackable)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bStackable), bStackable);
}

bool InventoryPrimaryAsset::GetPersistWhenEmpty(const FAssetData& AssetData, bool& bPersistWhenEmpty)
{
	return AssetData.GetTagValue<bool>(GET_MEMBER_NAME_CHECKED(UInventoryAsset, bPersistWhenEmpty), bPersistWhenEmpty);
}



const FName InventoryFilterProperty::AssetId(TEXT("AssetId"));
const FName InventoryFilterProperty::AssetType(TEXT("AssetType"));
const FName InventoryFilterProperty::AssetRarity(TEXT("AssetRarity"));

const FName InventoryFilterProperty::ItemId(TEXT("ItemId"));
const FName InventoryFilterProperty::ItemQuantity(TEXT("ItemQuantity"));
const FName InventoryFilterProperty::ItemExperience(TEXT("ItemExperience"));
const FName InventoryFilterProperty::ItemLevel(TEXT("ItemLevel"));
const FName InventoryFilterProperty::ItemRank(TEXT("ItemRank"));


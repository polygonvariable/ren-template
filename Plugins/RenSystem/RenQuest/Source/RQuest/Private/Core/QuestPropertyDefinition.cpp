// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/QuestPropertyDefinition.h"

// Project Headers
#include "Data/QuestAsset.h"


bool FQuestAssetData::IsValid(const FPrimaryAssetId& AssetId)
{
	return AssetId.PrimaryAssetType == UQuestAsset::GetPrimaryAssetType();
}

bool FQuestAssetData::GetTitle(const FAssetData& AssetData, FText& OutTitle)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UQuestAsset, Title), OutTitle);
}

bool FQuestAssetData::GetDescription(const FAssetData& AssetData, FText& OutDescription)
{
	return AssetData.GetTagValue<FText>(GET_MEMBER_NAME_CHECKED(UQuestAsset, Summary), OutDescription);
}

bool FQuestAssetData::GetQuestType(const FAssetData& AssetData, FName& OutType)
{
	return AssetData.GetTagValue<FName>(GET_MEMBER_NAME_CHECKED(UQuestAsset, QuestType), OutType);
}

bool FQuestAssetData::GetQuestType(const FAssetData& AssetData, EQuestType& OutType)
{
	FName Type;
	GetQuestType(AssetData, Type);

	const UEnum* Enum = StaticEnum<EQuestType>();
	int64 EnumValue = Enum->GetValueByName(Type);
	if (EnumValue == INDEX_NONE)
	{
		OutType = EQuestType::Story;
		return false;
	}

	OutType = static_cast<EQuestType>(EnumValue);
	return true;
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/QuestType.h"


/**
 *
 */
namespace FQuestAssetData
{
	RQUEST_API bool IsValid(const FPrimaryAssetId& AssetId);

	RQUEST_API bool GetTitle(const FAssetData& AssetData, FText& OutTitle);
	RQUEST_API bool GetDescription(const FAssetData& AssetData, FText& OutDescription);
	RQUEST_API bool GetQuestType(const FAssetData& AssetData, FName& OutType);
	RQUEST_API bool GetQuestType(const FAssetData& AssetData, EQuestType& OutType);
};


/**
 *
 */
namespace FQuestFilterProperty
{
	const FName QuestType = TEXT("QuestType");
};


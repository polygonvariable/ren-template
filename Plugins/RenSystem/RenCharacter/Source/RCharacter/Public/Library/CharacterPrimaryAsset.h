// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCHARACTER_API


/**
 *
 */
namespace FCharacterPrimaryAsset
{

	REN_API FPrimaryAssetType GetAssetType();
	REN_API FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	REN_API bool IsValid(const FPrimaryAssetId& AssetId);

	REN_API bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	REN_API bool GetHealth(const FAssetData& AssetData, int& Health);

};


// Module Macros
#undef REN_API


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCHARACTER_API



/**
 *
 */
class REN_API FCharacterPrimaryAsset
{

public:

	static FPrimaryAssetType GetAssetType();
	static FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	static bool IsValid(const FPrimaryAssetId& AssetId);

	static bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	static bool GetHealth(const FAssetData& AssetData, int& Health);

};


// Module Macros
#undef REN_API


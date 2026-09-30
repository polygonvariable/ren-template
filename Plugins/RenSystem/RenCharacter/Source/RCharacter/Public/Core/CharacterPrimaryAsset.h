// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/**
 *
 */
namespace FCharacterPrimaryAsset
{

	RCHARACTER_API FPrimaryAssetType GetAssetType();
	RCHARACTER_API FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	RCHARACTER_API bool IsValid(const FPrimaryAssetId& AssetId);

	RCHARACTER_API bool GetDisplayName(const FAssetData& AssetData, FText& DisplayName);
	RCHARACTER_API bool GetHealth(const FAssetData& AssetData, int& Health);

};


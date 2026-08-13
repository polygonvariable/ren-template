// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/CharacterAsset.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Core/Type/AscensionData.h"
#include "Core/Type/AssetRuleDefinition.h"
#include "Data/AssetCollection.h"
#include "Data/AssetGroup.h"


FPrimaryAssetId UCharacterAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(UCharacterAsset::GetPrimaryAssetType(), GetFName());
}

FPrimaryAssetType UCharacterAsset::GetPrimaryAssetType()
{
	return TEXT("Asset.Character");
}


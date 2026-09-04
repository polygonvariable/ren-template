// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EnvironmentProfileType.h"


/**
 *
 */
namespace FEnvironmentProfilePrimaryAsset
{

    FPrimaryAssetType GetAssetType();
	FPrimaryAssetId GetPrimaryAssetId(const FName& AssetName);

	bool IsValid(const FPrimaryAssetId& AssetId);

    bool GetType(const FAssetData& AssetData, FName& ProfileType);
    bool GetType(const FAssetData& AssetData, EEnvironmentProfileType& ProfileType);

};


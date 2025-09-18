// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Forward Declarations
class UAssetManager;



/**
 *
 */
class AssetManagerUtils
{

public:

	template <typename TReturnType>
	static TReturnType GetAssetTagValue(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, FName Tag);

    RCORELIBRARY_API static void LoadPrimaryAsset(UObject* Outer, UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, TFunction<void(FPrimaryAssetId, UObject*)> OnLoaded);

};


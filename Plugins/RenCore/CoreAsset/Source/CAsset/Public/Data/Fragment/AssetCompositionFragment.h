// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/AssetFragment.h"

// Generated Headers
#include "AssetCompositionFragment.generated.h"

// Forward Declarations
class UAssetGroup;
class UAssetCollection;


/**
 *
 */
UCLASS(Const)
class UAssetCompositionFragment : public UAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> BreakdownItems = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> RebuildItems = nullptr;


	CASSET_API virtual const UAssetCollection* GetBreakdownAssets(const FGuid& InId) const;
	CASSET_API virtual const UAssetCollection* GetRebuildAssets(const FGuid& InId) const;

};


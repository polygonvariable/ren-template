// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/AssetFragment.h"

// Generated Headers
#include "AssetCompositionFragment.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

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


	REN_API virtual const UAssetCollection* GetBreakdownAssets(const FGuid& InId) const;
	REN_API virtual const UAssetCollection* GetRebuildAssets(const FGuid& InId) const;

};


// Module Macros
#undef REN_API


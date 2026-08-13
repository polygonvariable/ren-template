// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Project Headers
#include "Delegate/GameEventDelegate.h" 

// Generated Headers
#include "AssetInstanceCollection.generated.h"

// Module Macros
#define REN_API RCOREASSETINSTANCE_API


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAssetInstanceCollection : public UInterface
{
	GENERATED_BODY()
};

/**
 * Asset instance is basically items created from asset.
 * like inventory item created from inventory asset
 */
class REN_API IAssetInstanceCollection
{

	GENERATED_BODY()

public:

	virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) = 0;
	virtual bool AddInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) { return false; };

	virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) = 0;
	virtual bool RemoveInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) { return false; };

	virtual bool RemoveAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) { return false; };
	virtual bool RemoveInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, int Quantity) { return false; };

	virtual bool ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const = 0;
	virtual bool ContainInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) const { return false; };
	virtual bool ContainAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const { return false; };

	virtual FGameEventDelegate& GetOnAssetInstanceCollectionUpdated() = 0;
	
};


// Module Macros
#undef REN_API


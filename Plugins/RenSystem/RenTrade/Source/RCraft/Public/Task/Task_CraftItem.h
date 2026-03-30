// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_CraftItem.generated.h"

// Forward Declarations
class URAssetManager;
class UTradeAsset;
class UCoreDataAsset;
class IAssetInstanceCollection;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_CraftItem : public UTaskObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId CraftAssetId;
	FPrimaryAssetId TargetAssetId;
	FGuid TradeCollectionId;

protected:

	int TargetQuantity = 1;
	int TargetQuota = 0;
	FTimespan CraftingTime;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> TargetAsset = nullptr;

	IAssetInstanceCollection* MaterialTransaction = nullptr;



	void Step_LoadAsset();
	void Step_CheckTargetAsset();
	void Step_CheckMaterialAsset();
	void Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_CheckCraftQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};


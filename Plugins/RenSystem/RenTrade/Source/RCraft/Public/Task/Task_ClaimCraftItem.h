// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Task/TaskObject.h"

// Generated Headers
#include "Task_ClaimCraftItem.generated.h"

// Forward Declarations
class URAssetManager;
class UTradeAsset;
class UCoreDataAsset;



/**
 *
 *
 *
 */
UCLASS(NotBlueprintType)
class UTask_ClaimCraftItem : public UTaskObject
{

	GENERATED_BODY()

public:

	FPrimaryAssetId CraftAssetId;
	FPrimaryAssetId TargetAssetId;
	FGuid TradeCollectionId;

protected:

	int TargetQuantity = 1;
	int ClaimQuantity = 0;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;


	void Step_LoadAsset();
	void Step_CheckTarget();
	void Step_CheckClaimable();
	void Step_PerformTransaction();

	// ~ UTaskObject
	void OnStarted() override;
	void OnStopped() override;
	void OnCleanup() override;
	// ~ End of UTaskObject

};


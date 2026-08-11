// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AACraftItem.generated.h"

// Forward Declarations
class UAssetManager;
class UTradeAsset;
class UCoreDataAsset;
class IAssetInstanceCollection;
struct FStreamableHandle;


/**
 *
 */
UCLASS(NotBlueprintType)
class UAACraftItem : public UAuthAction
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
	TObjectPtr<UAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> TargetAsset = nullptr;

	IAssetInstanceCollection* MaterialTransaction = nullptr;



	void Step_LoadAsset();
	void Step_HandleOnAssetsLoaded();
	void Step_CheckTargetAsset();
	void Step_CheckMaterialAsset();
	void Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_CheckCraftQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

private:

	TSharedPtr<FStreamableHandle> _AssetHandle;

};


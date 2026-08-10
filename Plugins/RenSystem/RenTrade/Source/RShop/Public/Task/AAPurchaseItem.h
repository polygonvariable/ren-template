// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AAPurchaseItem.generated.h"

// Forward Declarations
class URAssetManager;
class UShopAsset;
class UCoreDataAsset;
class IAssetInstanceCollection;


/**
 *
 */
UCLASS(NotBlueprintType)
class UAAPurchaseItem : public UAuthAction
{

	GENERATED_BODY()

public:

	FPrimaryAssetId ShopAssetId;
	FPrimaryAssetId TargetAssetId;
	FGuid TradeCollectionId;

protected:

	int TargetQuota = 1;
	int TargetQuantity = 1;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UShopAsset> ShopAsset = nullptr;

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> TargetAsset = nullptr;

	IAssetInstanceCollection* MaterialInstance = nullptr;


	void Step_LoadAsset();
	void Step_CheckTarget();
	void Step_CheckMaterial();
	void Step_CheckMaterialTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_CheckShopQuota(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);
	void Step_PerformTransaction(TMap<FPrimaryAssetId, int>&& MaterialAssetList, FPrimaryAssetType MaterialAssetType);

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};


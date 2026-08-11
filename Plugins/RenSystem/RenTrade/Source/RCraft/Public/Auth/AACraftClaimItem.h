// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AACraftClaimItem.generated.h"

// Forward Declarations
class UAssetManager;
class UTradeAsset;
class UCoreDataAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(NotBlueprintType)
class UAACraftClaimItem : public UAuthAction
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
	TObjectPtr<UAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<const UTradeAsset> TradeAsset = nullptr;


	void Step_LoadAsset();
	void Step_HandleOnAssetsLoaded();
	void Step_CheckTarget();
	void Step_CheckClaimable();
	void Step_PerformTransaction();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

private:

	TSharedPtr<FStreamableHandle> _AssetHandle;

};


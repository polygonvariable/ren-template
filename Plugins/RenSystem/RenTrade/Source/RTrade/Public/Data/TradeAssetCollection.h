// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/TradeAssetDetail.h"
#include "Management/AssetCollection.h"

// Generated Headers
#include "TradeAssetCollection.generated.h"

// Module Macros
#define REN_API RTRADE_API

// Forward Declarations
class FObjectPreSaveContext;


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Trade Referenced)")
class UTradeAssetCollection : public UAssetCollection
{

	GENERATED_BODY()

public:

	REN_API virtual const TMap<UCoreDataAsset*, FTradeAssetDetail>& GetAssetList() const;
	REN_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FTradeAssetDetail& OutDetail) const;

	// ~ UAssetCollection
	REN_API virtual bool GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const override;
	REN_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const override;
	REN_API virtual void GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const override;
	REN_API virtual void GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const override;
	REN_API virtual void GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const override;
	// ~ End of UAssetCollection

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Asset List (Editor)"))
	TArray<FTradeAssetDetail> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Asset List"))
	TMap<UCoreDataAsset*, FTradeAssetDetail> AssetList;

};


// Module Macros
#undef REN_API


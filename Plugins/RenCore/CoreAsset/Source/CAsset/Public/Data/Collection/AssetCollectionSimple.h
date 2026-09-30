// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/AssetDetail.h"
#include "Data/AssetCollection.h"

// Generated Headers
#include "AssetCollectionSimple.generated.h"

// Forward Declarations
class FObjectPreSaveContext;


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Simple)")
class UAssetCollectionSimple : public UAssetCollection
{

	GENERATED_BODY()

public:

	CASSET_API virtual const TMap<FPrimaryAssetId, FAssetDetail>& GetAssetList() const;

	// ~ UAssetCollection
	CASSET_API virtual bool GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const override;
	CASSET_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const override;
	CASSET_API virtual void GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const override;
	CASSET_API virtual void GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const override;
	CASSET_API virtual void GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const override;
	// ~ End of UAssetCollection

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Asset List (Editor)"))
	TArray<FAssetDetail> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Asset List"))
	TMap<FPrimaryAssetId, FAssetDetail> AssetList;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AssetDetail.h"
#include "Management/AssetCollection.h"

// Generated Headers
#include "AssetCollection_Unique.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

// Forward Declarations
class FObjectPreSaveContext;
class UCoreDataAsset;


/**
 *
 *
 */
UCLASS(MinimalAPI, DisplayName = "Collection (Unique)")
class UAssetCollection_Unique : public UAssetCollection
{

	GENERATED_BODY()

public:

	REN_API virtual const TMap<FPrimaryAssetId, FAssetDetail_Unique>& GetAssetList() const;

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
	TArray<FAssetDetail_Unique> AssetListEd;

#endif

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Asset List"))
	TMap<FPrimaryAssetId, FAssetDetail_Unique> AssetList;

};



// Module Macros
#undef REN_API


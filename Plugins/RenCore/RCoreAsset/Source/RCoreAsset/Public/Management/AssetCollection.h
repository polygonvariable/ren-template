// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "AssetCollection.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

// Forward Declarations
class UCoreDataAsset;
struct FAssetDetail;


/**
 *
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetCollection : public UObject
{

	GENERATED_BODY()

public:

	REN_API const FGuid& GetCollectionId() const;
	REN_API const FPrimaryAssetType& GetCollectionType() const;

	REN_API virtual bool GetRandomAsset(TPair<FPrimaryAssetId, FAssetDetail>& OutAsset) const;
	REN_API virtual bool GetAssetDetail(const FPrimaryAssetId& AssetId, FAssetDetail& OutDetail) const;
	REN_API virtual void GetAssetList(TMap<FPrimaryAssetId, FAssetDetail>& OutAssets) const;
	REN_API virtual void GetAssetList(TMap<FPrimaryAssetId, int>& OutAssets) const;
	REN_API virtual void GetAssetIds(TArray<FPrimaryAssetId>& OutAssets) const;

protected:

	UPROPERTY(EditDefaultsOnly)
	FGuid CollectionId;

	UPROPERTY(VisibleAnywhere, meta = (DisplayName = "Asset Type (Debug)"))
	FPrimaryAssetType AssetType;

};



// Module Macros
#undef REN_API


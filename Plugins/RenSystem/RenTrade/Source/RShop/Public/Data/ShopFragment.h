// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/AssetFragment.h"

// Generated Headers
#include "ShopFragment.generated.h"

// Forward Declarations
class UAssetGroup;
class UAssetCollection;
struct FInstancedStruct;


/**
 *
 */
UCLASS()
class UShopFragment : public UAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> PurchaseCost = nullptr;

	// ~ IShopProvider
	virtual const UAssetCollection* GetPurchaseCost() const;
	virtual const UAssetCollection* GetPurchaseCost(const FInstancedStruct& Context) const;
	// ~ End of IShopProvider

#if WITH_EDITORONLY_DATA
	// ~ UAssetFragment
	virtual void AppendAssetBundleData(FAssetBundleData& Data) override;
	// ~ End of UAssetFragment
#endif

};


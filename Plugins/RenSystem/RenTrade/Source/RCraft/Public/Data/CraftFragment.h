// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Object/AssetFragment.h"

// Generated Headers
#include "CraftFragment.generated.h"

// Forward Declarations
class UAssetGroup;
class UAssetCollection;
struct FInstancedStruct;


/**
 *
 */
UCLASS()
class UCraftFragment : public UAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FTimespan CraftingTime;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetGroup> CraftingItems = nullptr;

	// ~ ICraftProvider
	virtual FTimespan GetCraftingTime() const;
	virtual const UAssetCollection* GetCraftingMaterial() const;
	virtual const UAssetCollection* GetCraftingMaterial(const FInstancedStruct& Context) const;
	// ~ End of ICraftProvider

#if WITH_EDITORONLY_DATA
	// ~ UAssetFragment
	virtual void AppendAssetBundleData(FAssetBundleData& Data) override;
	// ~ End of UAssetFragment
#endif

};


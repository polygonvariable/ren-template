// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Components/NamedSlot.h"

// Project Headers
#include "Core/AssetWidget.h"

// Generated Headers
#include "AssetFilterSlot.generated.h"

// Forward Declarations
class UFilterGroup;
class UFragmentedDataAsset;
class UAssetEntry;
struct FFilterContext;


/**
 *
 */
UCLASS(MinimalAPI, Abstract)
class UAssetFilterSlot : public UNamedSlot, public IAssetWidget
{

	GENERATED_BODY()

public:

	// ~ IAssetWidget
	virtual void InitializeAssetDetail(const UFragmentedDataAsset* Asset) override {};
	virtual void InitializeEntryDetail(const UAssetEntry* Entry) override {};
	// ~ End of IAssetWidget

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterGroup> FilterGroup = nullptr;


	CASSETUI_API void Evaluate(const FFilterContext& Context);

};


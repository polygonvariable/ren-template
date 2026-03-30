// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Components/NamedSlot.h"

// Project Headers
#include "Interface/AssetWidget.h"

// Generated Headers
#include "AssetFilterSlot.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UFilterGroup;
class UCoreDataAsset;
class UAssetEntry;

struct FFilterContext;



/**
 *
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UAssetFilterSlot : public UNamedSlot, public IAssetWidget
{

	GENERATED_BODY()

public:

	// ~ IAssetWidget
	virtual void InitializeAssetDetail(const UCoreDataAsset* Asset) override {};
	virtual void InitializeEntryDetail(const UAssetEntry* Entry) override {};
	// ~ End of IAssetWidget

protected:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UFilterGroup> FilterGroup = nullptr;


	REN_API void Evaluate(const FFilterContext& Context);

};


// Module Macros
#undef REN_API


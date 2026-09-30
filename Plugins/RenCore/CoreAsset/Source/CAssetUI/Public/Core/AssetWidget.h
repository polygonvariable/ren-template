// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetWidget.generated.h"

// Forward Declarations
class UFragmentedDataAsset;
class UAssetEntry;



UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAssetWidget : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class CASSETUI_API IAssetWidget
{

	GENERATED_BODY()

public:

	virtual void InitializeAssetDetail(const UFragmentedDataAsset* Asset) = 0;
	virtual void InitializeEntryDetail(const UAssetEntry* Entry) = 0;

};


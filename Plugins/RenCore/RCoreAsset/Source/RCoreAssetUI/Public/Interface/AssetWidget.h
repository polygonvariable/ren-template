// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetWidget.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UCoreDataAsset;
class UAssetEntry;



UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAssetWidget : public UInterface
{

	GENERATED_BODY()

};

/**
 *
 */
class REN_API IAssetWidget
{

	GENERATED_BODY()

public:

	virtual void InitializeAssetDetail(const UCoreDataAsset* Asset) = 0;
	virtual void InitializeEntryDetail(const UAssetEntry* Entry) = 0;

};


// Module Macros
#undef REN_API


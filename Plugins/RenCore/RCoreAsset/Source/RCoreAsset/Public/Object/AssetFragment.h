// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetFragment.generated.h"

// Module Macros
#define REN_API RCOREASSET_API


/**
 * 
 */
UCLASS(Const, Abstract, MinimalAPI, EditInlineNew, DefaultToInstanced, CollapseCategories)
class UAssetFragment : public UObject
{
	
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	REN_API virtual void AppendAssetBundleData(FAssetBundleData& InAssetBundleData) {};
#endif

};


// Module Macros
#undef REN_API


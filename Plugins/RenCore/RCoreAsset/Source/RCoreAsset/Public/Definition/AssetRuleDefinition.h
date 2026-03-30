// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Generated Headers
#include "AssetRuleDefinition.generated.h"


USTRUCT()
struct FAssetRuleContext
{

	GENERATED_BODY()

public:

	FAssetRuleContext() {}
	FAssetRuleContext(const FGuid& InId) : Id(InId) {}

	FGuid Id;

};

USTRUCT()
struct FAssetRuleContext_List : public FAssetRuleContext
{

	GENERATED_BODY()

public:

	int Index = 0;
	
};


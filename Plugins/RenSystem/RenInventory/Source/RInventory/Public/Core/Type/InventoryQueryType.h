// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/AssetQuerySource.h"
#include "Core/Type/InventorySortType.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "InventoryQueryType.generated.h"


/**
 *
 */
USTRUCT(BlueprintType)
struct FInventoryQueryRule
{

	GENERATED_BODY()

public:

	FInventoryQueryRule() {}


	UPROPERTY(EditAnywhere)
	EAssetQuerySource QuerySource = EAssetQuerySource::Instance;

	UPROPERTY(EditAnywhere)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere)
	EInventorySortType SortType = EInventorySortType::Alphabetical;

};


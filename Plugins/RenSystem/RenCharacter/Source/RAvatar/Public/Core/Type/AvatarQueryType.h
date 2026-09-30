// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/QueryType.h"
#include "Core/Type/AssetQuerySource.h"
#include "Core/Type/AvatarSortType.h"

// Generated Headers
#include "AvatarQueryType.generated.h"


/**
 *
 * 
 */
USTRUCT(BlueprintType)
struct FAvatarQueryRule
{

	GENERATED_BODY()

public:

	FAvatarQueryRule() {}


	UPROPERTY(EditAnywhere)
	EAssetQuerySource QuerySource = EAssetQuerySource::Instance;

	UPROPERTY(EditAnywhere)
	ESortDirection SortDirection = ESortDirection::Ascending;

	UPROPERTY(EditAnywhere)
	EAvatarSortType SortType = EAvatarSortType::Alphabetical;

};


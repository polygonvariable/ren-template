// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AvatarSortType.generated.h"

// Forward Declarations
struct FAvatarInstance;


/**
 *
 */
UENUM(BlueprintType)
enum class EAvatarSortType : uint8
{
	None UMETA(DisplayName = "None"),
	Alphabetical UMETA(DisplayName = "Alphabetical"),
	Rank UMETA(DisplayName = "Rank"),
	Level UMETA(DisplayName = "Level"),
};


/**
 *
 */
struct FAvatarSortEntry
{

public:

	FAvatarSortEntry(FPrimaryAssetId InAssetId, FText InItemName) : AssetId(InAssetId), ItemName(InItemName) {}
	FAvatarSortEntry(FPrimaryAssetId InAssetId, FText InItemName, const FAvatarInstance* InInstance) : AssetId(InAssetId), ItemName(InItemName), Instance(InInstance) {}

	FPrimaryAssetId AssetId;
	FText ItemName;
	const FAvatarInstance* Instance = nullptr;

};


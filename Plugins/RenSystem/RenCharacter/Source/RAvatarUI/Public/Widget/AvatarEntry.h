// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetEntry.h"

// Generated Headers
#include "AvatarEntry.generated.h"

// Forward Declarations
struct FAvatarInstance;


/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarEntry : public UAssetEntry
{

	GENERATED_BODY()

public:

	const FAvatarInstance* AvatarInstance = nullptr;

	// ~ UAvatarEntry
	virtual FGuid GetAssetInstanceId() const override;
	virtual void ResetData() override;
	// ~ UAvatarEntry

};


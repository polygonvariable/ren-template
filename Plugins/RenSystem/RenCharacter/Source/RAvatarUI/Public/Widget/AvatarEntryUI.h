// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetEntryUI.h"

// Generated Headers
#include "AvatarEntryUI.generated.h"

// Forward Declarations
class UCoreDataAsset;


/**
 *
 */
UCLASS(Abstract)
class UAvatarEntryUI : public UAssetEntryUI
{

	GENERATED_BODY()

protected:

	// ~ UAssetEntryUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	// ~ End of UAssetEntryUI

};


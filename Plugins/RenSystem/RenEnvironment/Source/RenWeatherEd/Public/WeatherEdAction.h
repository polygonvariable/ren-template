// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

// Project Headers

// Generated Headers

// Forward Declarations



/**
 *
 *
 */
class FWeatherAssetActions : public FAssetTypeActions_Base
{

public:

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override;

};


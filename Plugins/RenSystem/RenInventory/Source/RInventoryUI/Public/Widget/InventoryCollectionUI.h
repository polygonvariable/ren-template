// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/InventoryQueryType.h"
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "InventoryCollectionUI.generated.h"

// Forward Declarations
class UInventoryStorageManager;


/**
 *
 */
UCLASS(Abstract)
class UInventoryCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionUI
	virtual void InitializeCollection() override;
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY(EditAnywhere)
	FInventoryQueryRule QueryRule;

	UPROPERTY()
	TObjectPtr<UInventoryStorageManager> StorageManager = nullptr;


	// ~ UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


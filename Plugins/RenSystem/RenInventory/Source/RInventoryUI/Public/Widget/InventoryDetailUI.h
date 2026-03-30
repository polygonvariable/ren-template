// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "InventoryDetailUI.generated.h"

// Forward Declarations
class UAscensionDetailUI;
class UInventoryStorage;
class UInventorySubsystem;
class UCoreDataAsset;

struct FInventoryInstance;



/**
 *
 * 
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDetailUI
	virtual void InitializeDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetDetailUI

protected:

	FGuid ActiveItemId;

	UPROPERTY()
	TWeakObjectPtr<UInventoryStorage> Inventory = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAscensionDetailUI> AscensionDetail = nullptr;


	virtual void SetCustomDetails(const FInventoryInstance* Item, int Quantity);

	// ~ UAssetDetailUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDetailUI

	// ~ End of UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


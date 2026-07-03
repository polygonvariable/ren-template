// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/TradeDashboardUI.h"

// Generated Headers
#include "ShopDashboardUI.generated.h"

// Forward Declarations
class UAssetCollection;
class UCoreDataAsset;
class UShopSubsystem;


/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UShopDashboardUI : public UTradeDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<UShopSubsystem> ShopSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ShopButton = nullptr;


	UFUNCTION()
	void HandlePurchase();

	// ~ UTradeDashboardUI
	virtual const UAssetCollection* GetTradeMaterialCollection(const UCoreDataAsset* Asset) const override;
	// ~ End of UTradeDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


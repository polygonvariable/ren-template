// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UAssetDetailUI;
class UAssetFilterCollectionUI;
class UAssetEntry;
class UInventoryAsset;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	virtual void ResetDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> InventoryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetFilterCollectionUI> InventoryFilter = nullptr;


	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	virtual void RedirectToWidget(TSubclassOf<UAssetDashboardUI> WidgetClass) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "TradeDashboardUI.generated.h"

// Module Macros
#define REN_API RTRADEUI_API

// Forward Declarations
class UAssetCollection;
class UAssetCollectionUI;
class UTradeCollectionUI;
class UTradeDetailUI;
class UAssetEntry;
class UTradeAsset;
class UCoreDataAsset;
struct FStreamableHandle;


/**
 *
 */
UCLASS(Abstract, MinimalAPI, NotBlueprintable)
class UTradeDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	REN_API virtual void InitializeDetail() override;
	REN_API virtual void ResetDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	FPrimaryAssetId TradeAssetId;

	UPROPERTY(EditAnywhere)
	FGuid TradeCollectionId;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeCollectionUI> PrimaryCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTradeDetailUI> PrimaryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> SecondaryCollection = nullptr;


	REN_API virtual void InitializeTradeDetail();
	REN_API virtual const UAssetCollection* GetTradeMaterialCollection(const UCoreDataAsset* Asset) const;

	// ~ UAssetDashboardUI
	REN_API virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	REN_API virtual void CancelInitialization() override;
	// ~ End of UAssetDashboardUI
	
	// ~ UUserWidget
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TSharedPtr<FStreamableHandle> _TradeHandle;

};



// Module Macros
#undef REN_API


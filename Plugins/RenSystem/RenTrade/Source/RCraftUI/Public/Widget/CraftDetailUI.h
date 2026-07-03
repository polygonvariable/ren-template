// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/CraftData.h"
#include "Widget/TradeDetailUI.h"

// Generated Headers
#include "CraftDetailUI.generated.h"

// Forward Declarations
class UTextBlock;
class UAssetEntry;
class UCoreDataAsset;
class UCraftStorageManager;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCraftDetailUI : public UTradeDetailUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDetailUI
	virtual void RefreshDetail() override;
	// ~ End of UAssetDetailUI

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CraftTime = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CraftQuantity = nullptr;

	UPROPERTY()
	TObjectPtr<UCraftStorageManager> StorageManager = nullptr;

	FCraftData CraftData;


	UFUNCTION(BlueprintCallable)
	virtual void RefreshCraftDetail();

	// ~ UAssetDetailUI
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDetailUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


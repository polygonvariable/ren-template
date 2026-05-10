// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/TradeCollectionUI.h"

// Generated Headers
#include "ShopCollectionUI.generated.h"

// Forward Declarations
class UShopSubsystem;
class UShopStorageManager;


/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UShopCollectionUI : public UTradeCollectionUI
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<UShopSubsystem> ShopSubsystem;

	UPROPERTY()
	TObjectPtr<UShopStorageManager> StorageManager;


	// ~ UTradeCollectionUI
	virtual void DisplayEntries() override;
	// ~ End of UTradeCollectionUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDetailUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UImage;
class UTextBlock;
class UWidgetSwitcher;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDetailUI : public UAssetUI
{

	GENERATED_BODY()

public:

	// ~ UAssetUI
	REN_API virtual void ResetDetail() override;
	// ~ End of UAssetUI

protected:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	bool bAutoRefresh = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EntryIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryName = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EntryDescription = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitch = nullptr;


	// ~ UAssetUI
	REN_API virtual void SwitchDetail(bool bPrimary);
	// ~ End of UAssetUI

};



// Module Macros
#undef REN_API


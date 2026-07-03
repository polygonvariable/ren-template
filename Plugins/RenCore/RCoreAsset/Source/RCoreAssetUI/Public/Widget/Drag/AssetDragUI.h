// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "AssetDragUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UImage;
class UTextBlock;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDragUI : public UUserWidget
{

	GENERATED_BODY()

public:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	// ~ End of UUserWidget

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AssetIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AssetDisplayName = nullptr;

};


// Module Macros
#undef REN_API


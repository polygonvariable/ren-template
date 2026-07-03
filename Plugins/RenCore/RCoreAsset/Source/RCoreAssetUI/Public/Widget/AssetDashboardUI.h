// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDashboardUI.generated.h"

// Module Macros
#define REN_API RCOREASSETUI_API

// Forward Declarations
class UButton;
class UOverlay;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UAssetDashboardUI : public UAssetUI
{

	GENERATED_BODY()

public:

	// ~ IAssetWidget
	REN_API virtual void InitializeAssetDetail(const UCoreDataAsset* Asset) override;
	REN_API virtual void InitializeEntryDetail(const UAssetEntry* Entry) override;
	// ~ End of IAssetWidget

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UOverlay> LoaderOverlay = nullptr;


	UFUNCTION(BlueprintNativeEvent)
	REN_API void GetAssetWidgets(TArray<UWidget*>& Widgets);
	REN_API virtual void GetAssetWidgets_Implementation(TArray<UWidget*>& Widgets);

	UFUNCTION(BlueprintCallable)
	REN_API virtual void RedirectToWidget(UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UAssetDashboardUI> WidgetClass);

	// ~ UAssetUI
	REN_API virtual void LockControls_Implementation() override;
	REN_API virtual void UnlockControls_Implementation() override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Module Macros
#undef REN_API


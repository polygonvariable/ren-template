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

	UPROPERTY(Meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton = nullptr;


	UFUNCTION(BlueprintNativeEvent)
	REN_API void GetAssetWidgets(TArray<UWidget*>& Widgets);
	REN_API virtual void GetAssetWidgets_Implementation(TArray<UWidget*>& Widgets);

	UFUNCTION(BlueprintCallable)
	REN_API virtual void RedirectToWidget(UPARAM(Meta = (AllowAbstract = false)) TSubclassOf<UAssetDashboardUI> WidgetClass);

	// ~ UUserWidget
	REN_API virtual void NativeConstruct() override;
	REN_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Module Macros
#undef REN_API


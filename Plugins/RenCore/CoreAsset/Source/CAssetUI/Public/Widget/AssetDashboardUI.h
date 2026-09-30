// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "AssetDashboardUI.generated.h"

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
	CASSETUI_API virtual void InitializeAssetDetail(const UFragmentedDataAsset* Asset) override;
	CASSETUI_API virtual void InitializeEntryDetail(const UAssetEntry* Entry) override;
	CASSETUI_API virtual void CloseWidget() override;
	// ~ End of IAssetWidget

protected:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UOverlay> LoaderOverlay = nullptr;

	UPROPERTY(EditAnywhere)
	bool bHideOnClose = true;


	UFUNCTION(BlueprintNativeEvent)
	CASSETUI_API void GetAssetWidgets(TArray<UWidget*>& Widgets);
	CASSETUI_API virtual void GetAssetWidgets_Implementation(TArray<UWidget*>& Widgets);

	UFUNCTION(BlueprintCallable)
	CASSETUI_API virtual void RedirectToWidget(UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UAssetDashboardUI> WidgetClass);

	// ~ UAssetUI
	CASSETUI_API virtual void LockControls_Implementation() override;
	CASSETUI_API virtual void UnlockControls_Implementation() override;
	// ~ End of UAssetUI

	// ~ UUserWidget
	CASSETUI_API virtual void NativeConstruct() override;
	CASSETUI_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


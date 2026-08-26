// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Widget/AssetUI.h"

// Generated Headers
#include "EquipmentSlotCollectionUI.generated.h"

// Forward Declarations
class UPanelWidget;
class UEquipmentSlotUI;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentSlotCollectionUI : public UAssetUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> SlotBox = nullptr;

	UPROPERTY(EditAnywhere, meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEquipmentSlotUI> SlotClass = nullptr;

	UPROPERTY(EditAnywhere)
	FMargin SlotSpacing;

	
	// ~ UAssetDashboardUI
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	// ~ End of UUserWidget

};


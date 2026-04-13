// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "EquipmentDashboardUI.generated.h"

// Forward Declarations
class UAssetCollectionUI;
class UAssetDetailUI;
class UEquipmentSlotCollectionUI;
class UEquipmentSubsystem;
class UEquipmentStorage;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:
	
	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> EquipmentCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> OwnerDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UEquipmentSlotCollectionUI> SlotCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SyncButton = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	FGuid OwnerInstanceId;


	UFUNCTION()
	void SyncEquipment();

	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


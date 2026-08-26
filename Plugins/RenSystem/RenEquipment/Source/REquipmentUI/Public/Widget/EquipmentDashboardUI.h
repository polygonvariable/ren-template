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
class UEquipmentStorageManager;


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
	virtual void RefreshDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> EquipmentCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> OwnerDetail = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEquipmentSlotCollectionUI> SlotCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ApplyButton = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentStorageManager> StorageManager = nullptr;

	FGuid OwnerInstanceId;


	UFUNCTION()
	void ApplyEquipmentToOwner();

	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


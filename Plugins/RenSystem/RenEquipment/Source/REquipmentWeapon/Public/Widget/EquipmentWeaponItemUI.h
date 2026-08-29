// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/EquipmentItemUI.h"

// Generated Headers
#include "EquipmentWeaponItemUI.generated.h"


/**
 *
 */
UCLASS(Abstract)
class UEquipmentWeaponItemUI : public UEquipmentItemUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTextBlock = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTextBlock = nullptr;


	void UpdateWeaponData();

	// ~ UEquipmentItemUI
	virtual void RegisterEquipmentController() override;
	virtual void UnregisterEquipmentController() override;

	virtual void SetDetail(UEquipmentController* Controller) override;
	virtual void RefreshDetail() override;
	virtual void ResetDetail() override;
	// ~ End of UEquipmentItemUI

};


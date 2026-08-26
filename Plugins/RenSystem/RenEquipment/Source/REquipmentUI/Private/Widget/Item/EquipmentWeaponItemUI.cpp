// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/Item/EquipmentWeaponItemUI.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "System/Controller/EquipmentStateController_Weapon.h"
#include "System/EquipmentController.h"


void UEquipmentWeaponItemUI::RegisterEquipmentController()
{
	Super::RegisterEquipmentController();

	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		Weapon->OnDataChanged.AddUObject(this, &UEquipmentWeaponItemUI::RefreshDetail);
	}
}

void UEquipmentWeaponItemUI::UnregisterEquipmentController()
{
	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		Weapon->OnDataChanged.RemoveAll(this);
	}

	Super::UnregisterEquipmentController();
}

void UEquipmentWeaponItemUI::SetDetail(UEquipmentController* Controller)
{
	Super::SetDetail(Controller);
	UpdateWeaponData();
}

void UEquipmentWeaponItemUI::RefreshDetail()
{
	Super::RefreshDetail();

	UpdateWeaponData();
}

void UEquipmentWeaponItemUI::ResetDetail()
{
	Super::ResetDetail();

	CurrentTextBlock->SetText(FText::FromString("--"));
	MaxTextBlock->SetText(FText::FromString("--"));
}

void UEquipmentWeaponItemUI::UpdateWeaponData()
{
	UEquipmentProjectileWeaponController* Weapon = GetEquipmentController<UEquipmentProjectileWeaponController>();
	if (IsValid(Weapon))
	{
		CurrentTextBlock->SetText(FText::AsNumber(Weapon->GetProjectileCount()));
		MaxTextBlock->SetText(FText::AsNumber(Weapon->GetMaxProjectileCount()));
	}
}


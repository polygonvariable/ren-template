// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EquipmentController_Skill.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "Core/EquipmentSettings.h"


bool UEquipmentController_Skill::ActivateEquipment()
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent();
	if (IsValid(ASC))
	{
		const FEquipmentSlotDefinition* SlotData = UEquipmentSettings::GetEquipmentSlotById(GetEquipmentData().SlotId);
		if (SlotData)
		{
			ASC->PressInputID(SlotData->InputId);
		}
	}
	return true;
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActivationInput.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentInputMapping.h"
#include "System/EquipmentController.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UEquipmentActivationInput::RegisterInput()
{
	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (!IsValid(InputComponent))
	{
		return;
	}

	for (const FEquipmentActivationBinding& Input : Inputs)
	{
		const FEquipmentSlotDefinition* SlotData = UEquipmentSettings::GetEquipmentSlotById(Input.SlotId);
		if (Input.IsValid() && SlotData)
		{
			InternalInputBinding(Input, SlotData, InputComponent);
		}
	}
}

void UEquipmentActivationInput::InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotDefinition* SlotData, UEnhancedInputComponent* InputComponent)
{
}


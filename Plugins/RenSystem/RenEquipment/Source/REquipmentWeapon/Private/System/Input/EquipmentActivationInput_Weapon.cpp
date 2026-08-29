// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActivationInput_Weapon.h"

// Engine Headers
#include "EnhancedInputComponent.h"

// Project Headers
#include "EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Log/LogMacro.h"


void UEquipmentActivationInput_Weapon::InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotDefinition* SlotData, UEnhancedInputComponent* InputComponent)
{
	FEnhancedInputActionEventBinding& PressedBinding = InputComponent->BindAction(Input.InputAction, ETriggerEvent::Started, this, &UEquipmentActivationInput_Weapon::HandleOnInputPressed, Input.SlotId);
	InputHandles.Add(PressedBinding.GetHandle());
}

void UEquipmentActivationInput_Weapon::HandleOnInputPressed(const FInputActionValue& Value, FEquipmentSlotId SlotId)
{
	UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerComponent();
	if (IsValid(EquipmentManager))
	{
		EquipmentManager->ActivateEquipmentById(SlotId);
		LOG_WARNING(LogTemp, TEXT("Weapon activation key pressed: %d"), SlotId.Id);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActivationInput_Skill.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "Core/Type/EquipmentSlotData.h"
#include "Data/EquipmentInputMapping.h"
#include "Log/LogMacro.h"


void UEquipmentActivationInput_Skill::InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotData* SlotData, UEnhancedInputComponent* InputComponent)
{
	FEnhancedInputActionEventBinding& PressedBinding = InputComponent->BindAction(Input.InputAction, ETriggerEvent::Started, this, &UEquipmentActivationInput_Skill::HandleOnInputPressed, SlotData->InputId);
	InputHandles.Add(PressedBinding.GetHandle());

	FEnhancedInputActionEventBinding& ReleasedBinding = InputComponent->BindAction(Input.InputAction, ETriggerEvent::Completed, this, &UEquipmentActivationInput_Skill::HandleOnInputReleased, SlotData->InputId);
	InputHandles.Add(ReleasedBinding.GetHandle());
}

void UEquipmentActivationInput_Skill::HandleOnInputPressed(const FInputActionValue& Value, int InputId)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->PressInputID(InputId);
		LOG_WARNING(LogTemp, TEXT("Skill activation key pressed: %d"), InputId);
	}
}

void UEquipmentActivationInput_Skill::HandleOnInputReleased(const FInputActionValue& Value, int InputId)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC))
	{
		ASC->ReleaseInputID(InputId);
		LOG_WARNING(LogTemp, TEXT("Skill activation key released: %d"), InputId);
	}
}


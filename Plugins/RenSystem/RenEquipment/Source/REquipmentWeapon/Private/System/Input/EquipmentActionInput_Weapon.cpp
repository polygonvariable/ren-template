// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActionInput_Weapon.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/EquipmentController.h"


void UEquipmentActionInput_Weapon::RegisterInput()
{
	Super::RegisterInput();
}

void UEquipmentActionInput_Weapon::UnregisterInput()
{
	Super::UnregisterInput();
}

void UEquipmentActionInput_Weapon::HandleOnInputPressed(const FInputActionValue& Value, int InputIdOffset)
{
	CurrentSlotInputId = -1;

	UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerComponent();
	if (!IsValid(EquipmentManager))
	{
		return;
	}

	UEquipmentController* Controller = EquipmentManager->GetActiveController();
	if (!IsValid(Controller))
	{
		return;
	}

	const FEquipmentInitializationData& Data = Controller->GetEquipmentData();
	const FEquipmentSlotDefinition* SlotDefinition = UEquipmentSettings::GetEquipmentSlotById(Data.SlotId);
	if (!SlotDefinition)
	{
		return;
	}

	CurrentSlotInputId = SlotDefinition->InputId;

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) && CurrentSlotInputId > 0)
	{
		const int ActiveInputId = CurrentSlotInputId + InputIdOffset;
		
		ASC->PressInputID(ActiveInputId);
		ActiveInputs.Add(InputIdOffset, ActiveInputId);

		LOG_WARNING(LogEquipment, TEXT("Weapon action key pressed: %d"), ActiveInputId);
	}
}

void UEquipmentActionInput_Weapon::HandleOnInputReleased(const FInputActionValue& Value, int InputIdOffset)
{
	int ActiveInputId = 0;
	ActiveInputs.RemoveAndCopyValue(InputIdOffset, ActiveInputId);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) && ActiveInputId > 0)
	{
		ASC->ReleaseInputID(ActiveInputId);

		LOG_WARNING(LogEquipment, TEXT("Weapon action key released: %d"), ActiveInputId);
	}
}

void UEquipmentActionInput_Weapon::InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent)
{
	FEnhancedInputActionEventBinding& PressedBinding = InputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this, &UEquipmentActionInput_Weapon::HandleOnInputPressed, Action.InputIdOffset);
	InputHandles.Add(PressedBinding.GetHandle());

	FEnhancedInputActionEventBinding& ReleasedBinding = InputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this, &UEquipmentActionInput_Weapon::HandleOnInputReleased, Action.InputIdOffset);
	InputHandles.Add(ReleasedBinding.GetHandle());
}


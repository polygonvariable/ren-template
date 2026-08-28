// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActionInput_Weapon.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentInputMapping.h"
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

void UEquipmentActionInput_Weapon::HandleOnEquipmentChanged()
{
	//CurrentSlotInputId = -1;

	//UEquipmentManagerComponent* EquipmentManager = GetEquipmentManagerComponent();
	//if (!IsValid(EquipmentManager))
	//{
	//	return;
	//}

	//UEquipmentController* Controller = EquipmentManager->GetActiveController();
	//if (!IsValid(Controller))
	//{
	//	return;
	//}

	//const FEquipmentInitializationData& Data = Controller->GetEquipmentData();
	//const FEquipmentSlotDefinition* SlotDefinition = UEquipmentSettings::GetEquipmentSlotById(Data.SlotId);
	//if (!SlotDefinition)
	//{
	//	return;
	//}

	//CurrentSlotInputId = SlotDefinition->InputId;
	//LOG_WARNING(LogEquipment, TEXT("Weapon slot input id updated: %d"), CurrentSlotInputId);
}

void UEquipmentActionInput_Weapon::HandleOnInputPressed(const FInputActionValue& Value, int InputIdOffset)
{
	CurrentActionInputId = -1;

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

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) && SlotDefinition->InputId > 0)
	{
		CurrentActionInputId = SlotDefinition->InputId + InputIdOffset;
		ASC->PressInputID(CurrentActionInputId);

		LOG_WARNING(LogEquipment, TEXT("Weapon action key pressed: %d"), CurrentActionInputId);
	}
}

void UEquipmentActionInput_Weapon::HandleOnInputReleased(const FInputActionValue& Value, int InputIdOffset)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) && CurrentActionInputId > 0)
	{
		ASC->ReleaseInputID(CurrentActionInputId);

		LOG_WARNING(LogEquipment, TEXT("Weapon action key released: %d"), CurrentActionInputId);
	}

	CurrentActionInputId = -1;
	LOG_WARNING(LogEquipment, TEXT("Weapon action input id removed: %d"), CurrentActionInputId);
}

void UEquipmentActionInput_Weapon::InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent)
{
	FEnhancedInputActionEventBinding& PressedBinding = InputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this, &UEquipmentActionInput_Weapon::HandleOnInputPressed, Action.InputIdOffset);
	InputHandles.Add(PressedBinding.GetHandle());

	FEnhancedInputActionEventBinding& ReleasedBinding = InputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this, &UEquipmentActionInput_Weapon::HandleOnInputReleased, Action.InputIdOffset);
	InputHandles.Add(ReleasedBinding.GetHandle());
}


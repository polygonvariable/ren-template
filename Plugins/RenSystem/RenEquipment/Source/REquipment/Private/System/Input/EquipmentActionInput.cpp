// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Input/EquipmentActionInput.h"

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


void UEquipmentActionInput::RegisterInput()
{
	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (!IsValid(InputComponent))
	{
		return;
	}

	for (const FEquipmentActionBinding& Action : Actions)
	{
		if (Action.IsValid())
		{
			InternalInputBinding(Action, InputComponent);
		}
	}
}

void UEquipmentActionInput::InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent)
{
}


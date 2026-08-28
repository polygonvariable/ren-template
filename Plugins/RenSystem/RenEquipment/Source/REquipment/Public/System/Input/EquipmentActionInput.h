// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"
#include "System/EquipmentInputHandler.h"

// Generated Headers
#include "EquipmentActionInput.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
class UAbilitySystemComponent;
class UEquipmentManagerComponent;
struct FEquipmentSlotDefinition;
struct FInputActionValue;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentActionInput : public UEquipmentInputHandler
{

	GENERATED_BODY()

public:

	TArray<FEquipmentActionBinding> Actions;


	// ~ UEquipmentInputHandler
	virtual void RegisterInput() override;
	// ~ End of UEquipmentInputHandler

protected:

	virtual void InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent);

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"
#include "System/EquipmentInputHandler.h"

// Generated Headers
#include "EquipmentActivationInput.generated.h"

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
class UEquipmentActivationInput : public UEquipmentInputHandler
{

	GENERATED_BODY()

public:
	
	TArray<FEquipmentActivationBinding> Inputs;


	// ~ UEquipmentInputHandler
	virtual void RegisterInput() override;
	// ~ End of UEquipmentInputHandler

protected:

	virtual void InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotDefinition* SlotData, UEnhancedInputComponent* InputComponent);

};


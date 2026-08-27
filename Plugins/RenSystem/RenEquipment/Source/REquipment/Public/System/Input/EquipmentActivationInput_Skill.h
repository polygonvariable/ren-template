// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EquipmentActivationInput.h"

// Generated Headers
#include "EquipmentActivationInput_Skill.generated.h"

// Forward Declarations
struct FInputActionValue;
struct FEquipmentActivationBinding;
struct FEquipmentSlotData;


/**
 *
 */
UCLASS()
class UEquipmentActivationInput_Skill : public UEquipmentActivationInput
{

	GENERATED_BODY()

protected:

	// ~ UEquipmentInputInteraction
	virtual void InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotData* SlotData, UEnhancedInputComponent* InputComponent) override;
	// ~ End of UEquipmentInputInteraction

	// ~ Input Bindings
	void HandleOnInputPressed(const FInputActionValue& Value, int InputId);
	void HandleOnInputReleased(const FInputActionValue& Value, int InputId);
	// ~ End of Input Binding

};


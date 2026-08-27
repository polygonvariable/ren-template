// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSlotId.h"
#include "System/EquipmentActivationInput.h"

// Generated Headers
#include "EquipmentActivationInput_Weapon.generated.h"

// Forward Declarations
struct FInputActionValue;
struct FEquipmentActivationBinding;
struct FEquipmentSlotData;


/**
 *
 */
UCLASS()
class UEquipmentActivationInput_Weapon : public UEquipmentActivationInput
{

	GENERATED_BODY()

protected:

	// ~ UEquipmentInputInteraction
	virtual void InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotData* SlotData, UEnhancedInputComponent* InputComponent) override;
	// ~ End of UEquipmentInputInteraction

	// ~ Input Bindings
	void HandleOnInputPressed(const FInputActionValue& Value, FEquipmentSlotId SlotId);
	// ~ End of Input Binding

};


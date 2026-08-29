// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"
#include "System/Input/EquipmentActionInput.h"

// Generated Headers
#include "EquipmentActionInput_Weapon.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
struct FInputActionValue;


/**
 *
 */
UCLASS()
class UEquipmentActionInput_Weapon : public UEquipmentActionInput
{

	GENERATED_BODY()

public:

	// ~ UEquipmentActionInput
	virtual void RegisterInput() override;
	virtual void UnregisterInput() override;
	// ~ End of UEquipmentActionInput

protected:

	TMap<int, int> ActiveInputs;

	int CurrentSlotInputId = -1;


	// ~ Binding
	void HandleOnInputPressed(const FInputActionValue& Value, int InputIdOffset);
	void HandleOnInputReleased(const FInputActionValue& Value, int InputIdOffset);
	// ~ End of Binding

	// ~ UEquipmentActionInput
	virtual void InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent) override;
	// ~ End of UEquipmentActionInput

};


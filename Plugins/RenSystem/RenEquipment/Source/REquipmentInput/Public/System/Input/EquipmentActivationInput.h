// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"
#include "System/EquipmentInputHandler.h"

// Generated Headers
#include "EquipmentActivationInput.generated.h"

// Module Macros
#define REN_API REQUIPMENTINPUT_API

// Forward Declarations
class UEnhancedInputComponent;
struct FEquipmentSlotDefinition;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentActivationInput : public UEquipmentInputHandler
{

	GENERATED_BODY()

public:
	
	TArray<FEquipmentActivationBinding> Inputs;


	// ~ UEquipmentInputHandler
	REN_API virtual void RegisterInput() override;
	// ~ End of UEquipmentInputHandler

protected:

	REN_API virtual void InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotDefinition* SlotData, UEnhancedInputComponent* InputComponent);

};


// Module Macros
#undef REN_API


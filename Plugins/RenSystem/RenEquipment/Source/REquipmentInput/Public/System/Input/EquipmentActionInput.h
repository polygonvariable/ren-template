// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"
#include "System/EquipmentInputHandler.h"

// Generated Headers
#include "EquipmentActionInput.generated.h"

// Module Macros
#define REN_API REQUIPMENTINPUT_API

// Forward Declarations
class UEnhancedInputComponent;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentActionInput : public UEquipmentInputHandler
{

	GENERATED_BODY()

public:

	TArray<FEquipmentActionBinding> Actions;


	// ~ UEquipmentInputHandler
	REN_API virtual void RegisterInput() override;
	// ~ End of UEquipmentInputHandler

protected:

	REN_API virtual void InternalInputBinding(const FEquipmentActionBinding& Action, UEnhancedInputComponent* InputComponent);

};


// Module Macros
#undef REN_API


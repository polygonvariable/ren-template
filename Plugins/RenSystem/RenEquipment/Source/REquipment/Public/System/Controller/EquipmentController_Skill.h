// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EquipmentController.h"

// Generated Headers
#include "EquipmentController_Skill.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API


/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable)
class UEquipmentController_Skill : public UEquipmentController
{

	GENERATED_BODY()

public:

	// ~ UEquipmentController
	virtual bool ActivateEquipment() override;
	// ~ End of UEquipmentController

};


// Module Macros
#undef REN_API


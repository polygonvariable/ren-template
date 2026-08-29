// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "System/EquipmentController.h"

// Generated Headers
#include "EquipmentController_Skill.generated.h"


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


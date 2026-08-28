// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"

// Generated Headers
#include "EquipmentInputMapping.generated.h"

// Forward Declarations
class UInputAction;
class UEquipmentActivationInput;
class UEquipmentActionInput;


/**
 *
 */
UCLASS(MinimalAPI)
class UEquipmentInputMapping : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Equipment Activation")
	TArray<FEquipmentActivationBinding> ActivationBindings;

	UPROPERTY(EditAnywhere, Category = "Equipment Activation")
	TSubclassOf<UEquipmentActivationInput> ActivationClass;

	UPROPERTY(EditAnywhere, Category = "Equipment Triggers")
	TArray<FEquipmentActionBinding> ActionBindings;

	UPROPERTY(EditAnywhere, Category = "Equipment Triggers")
	TSubclassOf<UEquipmentActionInput> ActionClass;

};


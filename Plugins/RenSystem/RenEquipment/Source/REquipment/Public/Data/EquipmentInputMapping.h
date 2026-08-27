// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Project Headers
#include "Core/Type/EquipmentSlotId.h"

// Generated Headers
#include "EquipmentInputMapping.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UInputAction;
class UEquipmentActivationInput;


/**
 *
 */
USTRUCT()
struct FEquipmentActivationBinding
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditAnywhere)
	FEquipmentSlotId SlotId;

	bool IsValid() const
	{
		return InputAction != nullptr && SlotId.IsValid();
	}

};


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

};


// Module Macros
#undef REN_API


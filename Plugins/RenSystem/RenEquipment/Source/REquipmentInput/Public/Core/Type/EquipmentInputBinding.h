// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSlotId.h"

// Generated Headers
#include "EquipmentInputBinding.generated.h"

// Forward Declarations
class UInputAction;


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
USTRUCT()
struct FEquipmentActionBinding
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 10))
	int InputIdOffset = 1;

	bool IsValid() const
	{
		return InputAction != nullptr && InputIdOffset > 0;
	}

};


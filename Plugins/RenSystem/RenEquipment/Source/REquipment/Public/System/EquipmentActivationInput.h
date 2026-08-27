// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "EquipmentActivationInput.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
class UAbilitySystemComponent;
class UEquipmentManagerComponent;
struct FEquipmentActivationBinding;
struct FEquipmentSlotData;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentActivationInput : public UObject
{

	GENERATED_BODY()

public:

	virtual void RegisterInput(const TArray<FEquipmentActivationBinding>& Inputs);
	virtual void UnregisterInput();

protected:

	TArray<uint8> InputHandles;


	APlayerController* GetOwner() const;
	UEnhancedInputComponent* GetInputComponent() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

	virtual void InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotData* SlotData, UEnhancedInputComponent* InputComponent);

};


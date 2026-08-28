// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"

// Generated Headers
#include "EquipmentInputHandler.generated.h"

// Forward Declarations
class UEnhancedInputComponent;
class UAbilitySystemComponent;
class UEquipmentManagerComponent;
struct FEquipmentSlotDefinition;
struct FInputActionValue;


/**
 *
 */
UCLASS(Abstract)
class UEquipmentInputHandler : public UObject
{

	GENERATED_BODY()

public:

	virtual void RegisterInput();
	virtual void UnregisterInput();

protected:

	TArray<uint8> InputHandles;


	APlayerController* GetOwner() const;
	UEnhancedInputComponent* GetInputComponent() const;
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

private:

	mutable TWeakObjectPtr<UEquipmentManagerComponent> _EquipmentManager;

};


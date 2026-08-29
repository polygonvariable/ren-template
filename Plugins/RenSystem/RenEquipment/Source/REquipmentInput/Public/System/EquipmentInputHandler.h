// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentInputBinding.h"

// Generated Headers
#include "EquipmentInputHandler.generated.h"

// Module Macros
#define REN_API REQUIPMENTINPUT_API

// Forward Declarations
class APawn;
class UEnhancedInputComponent;
class UAbilitySystemComponent;
class UEquipmentManagerComponent;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentInputHandler : public UObject
{

	GENERATED_BODY()

public:

	REN_API virtual void InitializeHandler();
	REN_API virtual void DeinitializeHandler();

	void RegisterPawn();
	void UnregisterPawn();

	REN_API virtual void RegisterInput();
	REN_API virtual void UnregisterInput();

protected:

	TArray<uint8> InputHandles;


	REN_API APlayerController* GetOwner() const;
	REN_API UEnhancedInputComponent* GetInputComponent() const;
	REN_API UAbilitySystemComponent* GetAbilitySystemComponent() const;
	REN_API UEquipmentManagerComponent* GetEquipmentManagerComponent() const;

	// ~ Binding
	UFUNCTION()
	REN_API virtual void HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn);
	// ~ End of Binding

};


// Module Macros
#undef REN_API


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentActivationInput.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentInputMapping.h"


void UEquipmentActivationInput::RegisterInput(const TArray<FEquipmentActivationBinding>& Inputs)
{
	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (!IsValid(InputComponent))
	{
		return;
	}

	for (const FEquipmentActivationBinding& Input : Inputs)
	{
		const FEquipmentSlotData* SlotData = UEquipmentSettings::GetEquipmentSlotById(Input.SlotId);
		if (Input.IsValid() && SlotData)
		{
			InternalInputBinding(Input, SlotData, InputComponent);
		}
	}
}

void UEquipmentActivationInput::UnregisterInput()
{
	UEnhancedInputComponent* InputComponent = GetInputComponent();
	if (!IsValid(InputComponent))
	{
		return;
	}

	for (const uint8& Handle : InputHandles)
	{
		InputComponent->RemoveBindingByHandle(Handle);
	}
}

void UEquipmentActivationInput::InternalInputBinding(const FEquipmentActivationBinding& Input, const FEquipmentSlotData* SlotData, UEnhancedInputComponent* InputComponent)
{
}

APlayerController* UEquipmentActivationInput::GetOwner() const
{
	UActorComponent* OwnerComponent = Cast<UActorComponent>(GetOuter());
	if (!IsValid(OwnerComponent))
	{
		return nullptr;
	}
	return OwnerComponent->GetOwner<APlayerController>();
}

UEnhancedInputComponent* UEquipmentActivationInput::GetInputComponent() const
{
	APlayerController* PC = GetOwner();
	if (!IsValid(PC))
	{
		return nullptr;
	}
	return Cast<UEnhancedInputComponent>(PC->InputComponent);
}

UAbilitySystemComponent* UEquipmentActivationInput::GetAbilitySystemComponent() const
{
	APlayerController* PC = GetOwner();
	if (!IsValid(PC))
	{
		return nullptr;
	}
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
}

UEquipmentManagerComponent* UEquipmentActivationInput::GetEquipmentManagerComponent() const
{
	APlayerController* PC = GetOwner();
	if (!IsValid(PC))
	{
		return nullptr;
	}

	APawn* Pawn = PC->GetPawn();
	if (!IsValid(Pawn))
	{
		return nullptr;
	}

	return Pawn->GetComponentByClass<UEquipmentManagerComponent>();
}


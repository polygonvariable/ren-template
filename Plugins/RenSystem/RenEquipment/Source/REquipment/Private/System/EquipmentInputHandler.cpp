// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EquipmentInputHandler.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"

// Project Headers
#include "AbilitySystemBlueprintLibrary.h"
#include "Component/EquipmentManagerComponent.h"
#include "Core/EquipmentSettings.h"
#include "Data/EquipmentInputMapping.h"
#include "System/EquipmentController.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


void UEquipmentInputHandler::RegisterInput()
{
}

void UEquipmentInputHandler::UnregisterInput()
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


APlayerController* UEquipmentInputHandler::GetOwner() const
{
	UActorComponent* OwnerComponent = Cast<UActorComponent>(GetOuter());
	if (!IsValid(OwnerComponent))
	{
		return nullptr;
	}
	return OwnerComponent->GetOwner<APlayerController>();
}

UEnhancedInputComponent* UEquipmentInputHandler::GetInputComponent() const
{
	APlayerController* PC = GetOwner();
	if (!IsValid(PC))
	{
		return nullptr;
	}
	return Cast<UEnhancedInputComponent>(PC->InputComponent);
}

UAbilitySystemComponent* UEquipmentInputHandler::GetAbilitySystemComponent() const
{
	APlayerController* PC = GetOwner();
	if (!IsValid(PC))
	{
		return nullptr;
	}
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PC->GetPawn());
}

UEquipmentManagerComponent* UEquipmentInputHandler::GetEquipmentManagerComponent() const
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


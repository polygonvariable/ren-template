// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EquipmentStateController.h"

// Engine Headers
#include "AbilitySystemComponent.h"

// Project Headers
#include "Data/EquipmentDataDefinition.h"


bool UEquipmentStateController::IsActive() const
{
	return _EquipmentState == EEquipmentState::Active;
}

bool UEquipmentStateController::IsActivating() const
{
	return _EquipmentState == EEquipmentState::Activating;
}

bool UEquipmentStateController::IsDeactivating() const
{
	return _EquipmentState == EEquipmentState::Deactivating;
}

bool UEquipmentStateController::IsTransitioning() const
{
	return _EquipmentState == EEquipmentState::Activating || _EquipmentState == EEquipmentState::Deactivating;
}

bool UEquipmentStateController::IsInactive() const
{
	return _EquipmentState == EEquipmentState::Inactive;
}


bool UEquipmentStateController::ActivateEquipment()
{
	return false;
}

bool UEquipmentStateController::DeactivateEquipment(bool bForce)
{
	return false;
}


EEquipmentState UEquipmentStateController::GetState() const
{
	return _EquipmentState;
}

void UEquipmentStateController::SetState(EEquipmentState InState)
{
	_EquipmentState = InState;
}


void UEquipmentStateController::CompleteActivation()
{
	_EquipmentState = EEquipmentState::Active;
}

void UEquipmentStateController::CompleteDeactivation()
{
	_EquipmentState = EEquipmentState::Inactive;
}


void UEquipmentStateController::HandleOnDeactivationTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount >= 0 && IsActive())
	{
		DeactivateEquipment();
	}
}


bool UEquipmentStateController::CanActivate() const
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (AbilitySystem->HasAnyMatchingGameplayTags(DataDefinition->DeactivationTag) || !IsInitialized())
	{
		return false;
	}
	return true;
}

void UEquipmentStateController::InitializeGameplayEvent()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(DataDefinition) && IsValid(AbilitySystem))
	{
		const FGameplayTagContainer& Tags = DataDefinition->DeactivationTag;
		for (const FGameplayTag& Tag : Tags)
		{
			if (Tag.IsValid())
			{
				AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UEquipmentStateController::HandleOnDeactivationTagChanged);
			}
		}
	}
}

void UEquipmentStateController::DeinitializeGameplayEvent()
{
	UAbilitySystemComponent* AbilitySystem = GetOwnerAbilitySystemComponent();
	if (IsValid(DataDefinition) && IsValid(AbilitySystem))
	{
		const FGameplayTagContainer& Tags = DataDefinition->DeactivationTag;
		for (const FGameplayTag& Tag : Tags)
		{
			if (Tag.IsValid())
			{
				AbilitySystem->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
			}
		}
	}
}



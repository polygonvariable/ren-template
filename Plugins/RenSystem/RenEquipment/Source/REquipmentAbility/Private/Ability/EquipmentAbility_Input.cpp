// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Ability/EquipmentAbility_Input.h"

// Engine Headers
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"


UEquipmentAbility_Input::UEquipmentAbility_Input()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}


void UEquipmentAbility_Input::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
    RegisterInput();
}

void UEquipmentAbility_Input::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    UnregisterInput();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UEquipmentAbility_Input::RegisterInput()
{
	_HoldReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	_HoldReleaseTask->OnRelease.AddDynamic(this, &UEquipmentAbility_Input::HandleOnInputHoldReleased);
	_HoldReleaseTask->ReadyForActivation();

	_HoldLockTask = UAbilityTask_WaitDelay::WaitDelay(this, HoldDelay);
	_HoldLockTask->OnFinish.AddDynamic(this, &UEquipmentAbility_Input::HandleOnInputHoldLocked);
	_HoldLockTask->ReadyForActivation();
}

void UEquipmentAbility_Input::UnregisterInput()
{
	if (IsValid(_HoldReleaseTask))
	{
		_HoldReleaseTask->EndTask();
		_HoldReleaseTask = nullptr;
	}

	if (IsValid(_HoldLockTask))
	{
		_HoldLockTask->EndTask();
		_HoldLockTask = nullptr;
	}
}


void UEquipmentAbility_Input::HandleOnInputHoldReleased(float TimeHeld)
{
	if (!IsActive())
	{
		return;
	}

	if (TimeHeld >= HoldDelay)
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
		return;
	}

	if (IsValid(_HoldLockTask))
	{
		_HoldLockTask->EndTask();
		_HoldLockTask = nullptr;
	}

	OnInputSingle();
}

void UEquipmentAbility_Input::HandleOnInputHoldLocked()
{
	if (!IsActive())
	{
		return;
	}

	if (IsValid(_HoldLockTask))
	{
		_HoldLockTask->EndTask();
		_HoldLockTask = nullptr;
	}

	OnInputHold();
}


void UEquipmentAbility_Input::OnInputSingle()
{
}

void UEquipmentAbility_Input::OnInputHold()
{
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/Controller/EquipmentController_Skill.h"

// Engine Headers
#include "AbilitySystemComponent.h"


bool UEquipmentController_Skill::ActivateEquipment()
{
	UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent();
	if (IsValid(ASC))
	{
		for (const FGameplayAbilitySpecHandle& Handle : ActiveAbilityHandles)
		{
			ASC->TryActivateAbility(Handle);
		}
	}

	return true;
}


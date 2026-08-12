// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EquipmentAbilityCollection.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"
#include "Abilities/GameplayAbility.h"


void UEquipmentAbilityCollection::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	//Abilities.Empty();
	//for (TSubclassOf<UGameplayAbility>& Ability : AbilityClasses)
	//{
	//	if (Ability)
	//	{
	//		FEquipmentAbilityData AbilityData;
	//		AbilityData.AbilityClass = Ability;
	//		Abilities.Add(AbilityData);
	//	}
	//}
#endif
}


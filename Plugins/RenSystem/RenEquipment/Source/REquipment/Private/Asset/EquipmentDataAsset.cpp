// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Asset/EquipmentDataAsset.h"

// Engine Headers
#include "UObject/ObjectSaveContext.h"


void UEquipmentAbilityCollection::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

#if WITH_EDITOR
	//if (Abilities.IsValidIndex(0))
	//{
	//	Abilities[0].EventTag = FGameplayTag::EmptyTag;
	//}
#endif
}


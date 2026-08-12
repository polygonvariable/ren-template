// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EquipmentFragment.h"

// Project Headers
#include "Data/EquipmentDataDefinition.h"


const UEquipmentDataDefinition* UEquipmentFragment::GetEquipmentDefinition() const
{
	if (!IsValid(EquipmentDefinition))
	{
		return nullptr;
	}
	return EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
}

#if WITH_EDITOR
void UEquipmentFragment::AppendAssetBundleData(FAssetBundleData& Data)
{
	if (IsValid(EquipmentDefinition))
	{
		UEquipmentDataDefinition* DefinitionCDO = EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
		if (IsValid(DefinitionCDO))
		{
			DefinitionCDO->AppendAssetBundleData(Data);
		}
	}
}
#endif


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/EquipmentSettings.h"

// Project Headers
#include "Data/EquipmentStorage.h"
#include "System/EquipmentStorageManager.h"
#include "System/EquipmentSubsystem.h"


UEquipmentSettings::UEquipmentSettings(const FObjectInitializer& ObjectInitializer)
{
	CategoryName = TEXT("Ren Project");

	StorageId = TEXT("equipment001");
	StorageClass = UEquipmentStorage::StaticClass();
	StorageManagerClass = UEquipmentStorageManager::StaticClass();

	SubsystemClass = UEquipmentSubsystem::StaticClass();
}

const UEquipmentSettings* UEquipmentSettings::Get()
{
	return GetDefault<UEquipmentSettings>();
}

const FEquipmentTagData* UEquipmentSettings::GetTagData(const FGameplayTag& Tag)
{
	return Get()->EquipmentTagRelations.Find(Tag);
}

const FEquipmentTagData* UEquipmentSettings::GetTagDataByAbility(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return nullptr;
	}

	const TMap<FGameplayTag, FEquipmentTagData>& Relations = Get()->EquipmentTagRelations;
	for (const TPair<FGameplayTag, FEquipmentTagData>& Kv : Relations)
	{
		if (Kv.Value.AbilityTag == Tag)
		{
			return &Kv.Value;
		}
	}
	return nullptr;
}

const FGameplayTag& UEquipmentSettings::GetAttachmentNotifyTag(bool bEquip)
{
	if (bEquip)
	{
		return Get()->EquipmentEquipNotify;
	}
	else
	{
		return Get()->EquipmentUnequipNotify;
	}
}


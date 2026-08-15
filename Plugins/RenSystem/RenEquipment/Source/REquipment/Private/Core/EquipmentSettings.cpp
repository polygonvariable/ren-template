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

	EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.01"));
	EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.02"));
	EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.01"));
	EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.02"));

	EquipmentDataTag = FGameplayTag::RequestGameplayTag("Equipment.Data");

	EquipmentTagRelations.Add(
		TPair<FGameplayTag, FEquipmentTagData>(
			FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.01"),
			FEquipmentTagData(
				FGameplayTag::RequestGameplayTag("Ability.Equipment.Weapon.Slot.01"),
				FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Slot.01"),
				FGameplayTag::RequestGameplayTag("State.Equipment.Weapon.Slot.01"),
				TEXT("Weapon01")
			)
		)
	);
	EquipmentTagRelations.Add(
		TPair<FGameplayTag, FEquipmentTagData>(
			FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.02"),
			FEquipmentTagData(
				FGameplayTag::RequestGameplayTag("Ability.Equipment.Weapon.Slot.02"),
				FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Slot.02"),
				FGameplayTag::RequestGameplayTag("State.Equipment.Weapon.Slot.02"),
				TEXT("Weapon02")
			)
		)
	);
	EquipmentTagRelations.Add(
		TPair<FGameplayTag, FEquipmentTagData>(
			FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.01"),
			FEquipmentTagData(
				FGameplayTag::RequestGameplayTag("Ability.Equipment.Skill.Slot.01"),
				FGameplayTag::RequestGameplayTag("Event.Equipment.Skill.Slot.01"),
				FGameplayTag::RequestGameplayTag("State.Equipment.Skill.Slot.01"),
				TEXT("Skill01")
			)
		)
	);
	EquipmentTagRelations.Add(
		TPair<FGameplayTag, FEquipmentTagData>(
			FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.02"),
			FEquipmentTagData(
				FGameplayTag::RequestGameplayTag("Ability.Equipment.Skill.Slot.02"),
				FGameplayTag::RequestGameplayTag("Event.Equipment.Skill.Slot.02"),
				FGameplayTag::RequestGameplayTag("State.Equipment.Skill.Slot.02"),
				TEXT("Skill02")
			)
		)
	);

	EquipmentEquipNotify = FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Attach");
	EquipmentUnequipNotify = FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Remove");
}

const UEquipmentSettings* UEquipmentSettings::Get()
{
	return GetDefault<UEquipmentSettings>();
}

const FEquipmentTagData* UEquipmentSettings::GetTagData(const FGameplayTag& Tag)
{
	const UEquipmentSettings* Settings = Get();
	return Settings->EquipmentTagRelations.Find(Tag);
}

const FEquipmentTagData* UEquipmentSettings::GetTagDataByAbility(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return nullptr;
	}

	const UEquipmentSettings* Settings = Get();
	const TMap<FGameplayTag, FEquipmentTagData>& Relations = Settings->EquipmentTagRelations;
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
	const UEquipmentSettings* Settings = Get();
	if (bEquip)
	{
		return Settings->EquipmentEquipNotify;
	}
	else
	{
		return Settings->EquipmentUnequipNotify;
	}
}


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

	EquipmentBundles.Add(TEXT("Equipment"));


	FEquipmentCategory CategoryWeapon(FGameplayTag::RequestGameplayTag("Equipment.Category.Weapon"), 2);
	CategoryWeapon.EquipSockets.Add(FEquipmentSocket(10, false, TEXT("rifleSocket_r")));
	CategoryWeapon.EquipSockets.Add(FEquipmentSocket(20, false, TEXT("rifleSocket_r")));
	CategoryWeapon.UnequipSockets.Add(FEquipmentSocket(10, true, TEXT("Weapon01")));
	CategoryWeapon.UnequipSockets.Add(FEquipmentSocket(20, true, TEXT("Weapon02")));

	FEquipmentCategory CategorySkill(FGameplayTag::RequestGameplayTag("Equipment.Category.Skill"), 3);

	EquipmentCategory.Add(CategoryWeapon);
	EquipmentCategory.Add(CategorySkill);



	//EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.01"));
	//EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.02"));
	//EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.03"));
	//EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.01"));
	//EquipmentSlots.AddTag(FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.02"));

	//EquipmentDataTag = FGameplayTag::RequestGameplayTag("Equipment.Data");

	//EquipmentTagRelations.Add(
	//	TPair<FGameplayTag, FEquipmentTagData>(
	//		FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.01"),
	//		FEquipmentTagData(
	//			FGameplayTag::RequestGameplayTag("Ability.Equipment.Weapon.Slot.01"),
	//			FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Slot.01"),
	//			FGameplayTag::RequestGameplayTag("State.Equipment.Weapon.Slot.01"),
	//			TEXT("Weapon01")
	//		)
	//	)
	//);
	//EquipmentTagRelations.Add(
	//	TPair<FGameplayTag, FEquipmentTagData>(
	//		FGameplayTag::RequestGameplayTag("Equipment.Weapon.Slot.02"),
	//		FEquipmentTagData(
	//			FGameplayTag::RequestGameplayTag("Ability.Equipment.Weapon.Slot.02"),
	//			FGameplayTag::RequestGameplayTag("Event.Equipment.Weapon.Slot.02"),
	//			FGameplayTag::RequestGameplayTag("State.Equipment.Weapon.Slot.02"),
	//			TEXT("Weapon02")
	//		)
	//	)
	//);
	//EquipmentTagRelations.Add(
	//	TPair<FGameplayTag, FEquipmentTagData>(
	//		FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.01"),
	//		FEquipmentTagData(
	//			FGameplayTag::RequestGameplayTag("Ability.Equipment.Skill.Slot.01"),
	//			FGameplayTag::RequestGameplayTag("Event.Equipment.Skill.Slot.01"),
	//			FGameplayTag::RequestGameplayTag("State.Equipment.Skill.Slot.01"),
	//			TEXT("Skill01")
	//		)
	//	)
	//);
	//EquipmentTagRelations.Add(
	//	TPair<FGameplayTag, FEquipmentTagData>(
	//		FGameplayTag::RequestGameplayTag("Equipment.Skill.Slot.02"),
	//		FEquipmentTagData(
	//			FGameplayTag::RequestGameplayTag("Ability.Equipment.Skill.Slot.02"),
	//			FGameplayTag::RequestGameplayTag("Event.Equipment.Skill.Slot.02"),
	//			FGameplayTag::RequestGameplayTag("State.Equipment.Skill.Slot.02"),
	//			TEXT("Skill02")
	//		)
	//	)
	//);

	EquipmentAttachNotify = FGameplayTag::RequestGameplayTag("Event.Equipment.Attach");
	EquipmentDetachNotify = FGameplayTag::RequestGameplayTag("Event.Equipment.Detach");
}

const FEquipmentSocket* UEquipmentSettings::GetSocketBySlotId(FGameplayTag CategoryTag, int SlotId, bool bEquip)
{
	const UEquipmentSettings* Settings = Get();
	const FEquipmentCategory* Category = nullptr;

	const TArray<FEquipmentCategory>& Categories = Settings->EquipmentCategory;
	for (const FEquipmentCategory& Item : Categories)
	{
		if (Item.CategoryTag == CategoryTag)
		{
			Category = &Item;
			break;
		}
	}

	if (!Category)
	{
		return nullptr;
	}

	const FEquipmentSocket* Socket = nullptr;

	const TArray<FEquipmentSocket>& Sockets = (bEquip) ? Category->EquipSockets : Category->UnequipSockets;
	for (const FEquipmentSocket& Item : Sockets)
	{
		if (Item.SlotId == SlotId)
		{
			Socket = &Item;
			break;
		}
	}

	return Socket;
}

const FEquipmentSocket* UEquipmentSettings::GetSocketBySlotIndex(FGameplayTag CategoryTag, int SlotIndex, bool bEquip)
{
	const UEquipmentSettings* Settings = Get();
	const FEquipmentCategory* Category = nullptr;

	const TArray<FEquipmentCategory>& Categories = Settings->EquipmentCategory;
	for (const FEquipmentCategory& Item : Categories)
	{
		if (Item.CategoryTag == CategoryTag)
		{
			Category = &Item;
			break;
		}
	}

	if (!Category)
	{
		return nullptr;
	}

	const TArray<FEquipmentSocket>& TargetArray = (bEquip) ? Category->EquipSockets : Category->UnequipSockets;
	if (!TargetArray.IsValidIndex(SlotIndex))
	{
		return nullptr;
	}

	return &TargetArray[SlotIndex];
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

const FGameplayTag& UEquipmentSettings::GetAttachmentEventTag(bool bEquip)
{
	const UEquipmentSettings* Settings = Get();
	if (bEquip)
	{
		return Settings->EquipmentAttachNotify;
	}
	else
	{
		return Settings->EquipmentDetachNotify;
	}
}


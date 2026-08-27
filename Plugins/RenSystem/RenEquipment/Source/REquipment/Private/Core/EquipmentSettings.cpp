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

	EquipmentType = TEXT("Asset.Inventory");
	OwnerType = TEXT("Asset.Character");

	EquipmentBundles.Add(TEXT("Equipment"));

	EquipmentCategories.Add(
		FEquipmentCategoryData(
			FGameplayTag::RequestGameplayTag("Equipment.Category.Weapon"),
			{
				FEquipmentSlotData(10, 110, { false, TEXT("rifleSocket_r") }, { true, TEXT("Weapon01") }),
				FEquipmentSlotData(20, 120, { false, TEXT("rifleSocket_r") }, { true, TEXT("Weapon02") })
			}
		)
	);
	EquipmentCategories.Add(
		FEquipmentCategoryData(
			FGameplayTag::RequestGameplayTag("Equipment.Category.Skill"),
			{
				FEquipmentSlotData(10, 210),
				FEquipmentSlotData(20, 220),
				FEquipmentSlotData(30, 230)
			}
		)
	);

	EquipmentAttachNotify = FGameplayTag::RequestGameplayTag("Equipment.Event.Attach");
	EquipmentDetachNotify = FGameplayTag::RequestGameplayTag("Equipment.Event.Detach");

	EquipmentCooldownTag = FGameplayTag::RequestGameplayTag("Equipment.Cooldown");
}

const FEquipmentCategoryData* UEquipmentSettings::GetEquipmentCategoryByTag(FGameplayTag CategoryTag)
{
	const UEquipmentSettings* Settings = Get();

	return Settings->EquipmentCategories.FindByPredicate([CategoryTag](FEquipmentCategoryData Item) { return Item.CategoryTag == CategoryTag; });
}

const FEquipmentSlotData* UEquipmentSettings::GetEquipmentSlotById(FGameplayTag CategoryTag, int Id)
{
	const FEquipmentCategoryData* Category = UEquipmentSettings::GetEquipmentCategoryByTag(CategoryTag);
	if (!Category)
	{
		return nullptr;
	}

	return Category->Slots.FindByPredicate([Id](FEquipmentSlotData Item) { return Item.SlotId == Id; });
}

const FEquipmentSlotData* UEquipmentSettings::GetEquipmentSlotById(const FEquipmentSlotId& SlotId)
{
	return GetEquipmentSlotById(SlotId.SlotTag, SlotId.Id);
}

const UEquipmentSettings* UEquipmentSettings::Get()
{
	return GetDefault<UEquipmentSettings>();
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


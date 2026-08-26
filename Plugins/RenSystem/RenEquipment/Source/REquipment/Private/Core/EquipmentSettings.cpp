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
				FEquipmentSlotData(10, { false, TEXT("rifleSocket_r") }, { true, TEXT("Weapon01") }),
				FEquipmentSlotData(20, { false, TEXT("rifleSocket_r") }, { true, TEXT("Weapon02") })
			}
		)
	);
	EquipmentCategories.Add(
		FEquipmentCategoryData(
			FGameplayTag::RequestGameplayTag("Equipment.Category.Skill"),
			{
				FEquipmentSlotData(10),
				FEquipmentSlotData(20),
				FEquipmentSlotData(30)
			}
		)
	);

	EquipmentAttachNotify = FGameplayTag::RequestGameplayTag("Equipment.Event.Attach");
	EquipmentDetachNotify = FGameplayTag::RequestGameplayTag("Equipment.Event.Detach");

}

const FEquipmentCategoryData* UEquipmentSettings::GetEquipmentCategoryByTag(FGameplayTag CategoryTag)
{
	const UEquipmentSettings* Settings = Get();

	return Settings->EquipmentCategories.FindByPredicate([CategoryTag](FEquipmentCategoryData Item) { return Item.CategoryTag == CategoryTag; });
}

const FEquipmentSlotData* UEquipmentSettings::GetEquipmentSlotById(FGameplayTag CategoryTag, int SlotId)
{
	const FEquipmentCategoryData* Category = UEquipmentSettings::GetEquipmentCategoryByTag(CategoryTag);
	if (!Category)
	{
		return nullptr;
	}

	return Category->Slots.FindByPredicate([SlotId](FEquipmentSlotData Item) { return Item.SlotId == SlotId; });
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


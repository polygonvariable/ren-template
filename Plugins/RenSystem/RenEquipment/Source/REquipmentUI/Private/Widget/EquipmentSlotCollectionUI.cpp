// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/EquipmentSlotCollectionUI.h"

// Engine Headers
#include "Components/PanelWidget.h"

// Project Headers
#include "Core/EquipmentSettings.h"
#include "Core/Type/EquipmentSlotId.h"
#include "Widget/AssetEntry.h"
#include "Widget/EquipmentSlotUI.h"


void UEquipmentSlotCollectionUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	TArray<UWidget*> Children = SlotBox->GetAllChildren();
	for (UWidget* Child : Children)
	{
		UAssetUI* SlotUI = Cast<UAssetUI>(Child);
		if (IsValid(SlotUI))
		{
			SlotUI->InitializeEntryDetail(Entry);
		}
	}
}

void UEquipmentSlotCollectionUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	if (IsValid(SlotClass))
	{
		const FEquipmentCategoryDefinition* Category = UEquipmentSettings::GetEquipmentCategoryByTag(CategoryTag);
		if (Category)
		{
			const TArray<FEquipmentSlotDefinition>& Slots = Category->Slots;

			int NumSlots = Slots.Num();
			for (int i = 0; i < NumSlots; i++)
			{
				UEquipmentSlotUI* SlotUI = CreateWidget<UEquipmentSlotUI>(this, SlotClass);
				if (IsValid(SlotUI))
				{
					SlotUI->SlotId = FEquipmentSlotId(CategoryTag, Slots[i].SlotId);

					SlotUI->SetPadding(SlotSpacing);
					SlotBox->AddChild(SlotUI);
				}
			}
		}
	}
}


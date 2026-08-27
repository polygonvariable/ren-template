// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "IPropertyTypeCustomization.h"

// Forward Declaration
struct FEquipmentSlotId;


/*
 *
 */
class FEquipmentSlotIdCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// ~ IPropertyTypeCustomization
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	// ~ End of IPropertyTypeCustomization

	//TArray<TSharedPtr<FEquipmentSlotId>> SlotOptions;
	//TSharedPtr<SComboBox<TSharedPtr<FEquipmentSlotId>>> SlotComboBox;
	//TSharedPtr<IPropertyHandle> SlotIdProperty;

	TSharedPtr<IPropertyHandle> SlotTag_Property;
	TSharedPtr<IPropertyHandle> Id_Property;

};


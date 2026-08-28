// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EquipmentSocketDefinitionCustomization.h"

// Engine Headers
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "SGameplayTagCombo.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"

// Project Headers
#include "Core/Type/EquipmentSocketDefinition.h"


TSharedRef<IPropertyTypeCustomization> FEquipmentSocketDefinitionCustomization::MakeInstance()
{
    return MakeShared<FEquipmentSocketDefinitionCustomization>();
}

void FEquipmentSocketDefinitionCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    TSharedPtr<IPropertyHandle> UseComponent_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEquipmentSocketDefinition, bUseComponent));
    TSharedPtr<IPropertyHandle> SocketName_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEquipmentSocketDefinition, SocketName));

    FSlateFontInfo BoldFont = StructCustomizationUtils.GetRegularFont();
    BoldFont.Size = 8;
    BoldFont.TypefaceFontName = FName("Bold");

    HeaderRow
        .NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        .MaxDesiredWidth(0.0f)
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(FMargin(0.0f, 6.0f))
                [
                    SNew(SGridPanel)
                        + SGridPanel::Slot(0, 0)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(UseComponent_Property->GetPropertyDisplayName())
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 0)
                        .Padding(12.0f, 0.0f, 0.0f, 0.0f)
                        [
                            UseComponent_Property->CreatePropertyValueWidget()
                        ]
                        + SGridPanel::Slot(0, 1)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(SocketName_Property->GetPropertyDisplayName())
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 1)
                        .Padding(12.0f, 0.0f, 0.0f, 0.0f)
                        [
                            SocketName_Property->CreatePropertyValueWidget()
                        ]
                ]
        ];
}

void FEquipmentSocketDefinitionCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    
}


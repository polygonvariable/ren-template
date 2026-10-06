// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowTransitionCustomization.h"

// Engine Headers
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SUniformGridPanel.h"

// Project Headers
#include "Type/EventflowTransition.h"


TSharedRef<IPropertyTypeCustomization> FEventflowTransitionCustomization::MakeInstance()
{
    return MakeShared<FEventflowTransitionCustomization>();
}

void FEventflowTransitionCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    TSharedPtr<IPropertyHandle> Result_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEventflowTransition, Result));
    TSharedPtr<IPropertyHandle> Type_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEventflowTransition, Type));

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
                .Padding(FMargin(0.0f, 6.0f))
                [
                    SNew(SGridPanel)
                        + SGridPanel::Slot(0, 0)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("On")))
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 0)
                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                        [
                            Result_Property->CreatePropertyValueWidget()
                        ]
                        + SGridPanel::Slot(0, 1)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(FText::FromString(TEXT("Transition To")))
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 1)
                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                        [
                            Type_Property->CreatePropertyValueWidget()
                        ]
                ]
        ];
}

void FEventflowTransitionCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    
}


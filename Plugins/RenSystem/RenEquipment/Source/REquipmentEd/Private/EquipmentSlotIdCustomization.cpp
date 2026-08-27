// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EquipmentSlotIdCustomization.h"

// Engine Headers
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "SGameplayTagCombo.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Text/STextBlock.h"

// Project Headers
#include "Core/Type/EquipmentSlotId.h"
#include "Core/EquipmentSettings.h"


TSharedRef<IPropertyTypeCustomization> FEquipmentSlotIdCustomization::MakeInstance()
{
    return MakeShared<FEquipmentSlotIdCustomization>();
}

void FEquipmentSlotIdCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    SlotTag_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEquipmentSlotId, SlotTag));
    Id_Property = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FEquipmentSlotId, Id));

    FSlateFontInfo BoldFont = StructCustomizationUtils.GetRegularFont();
    BoldFont.Size = 8;
    BoldFont.TypefaceFontName = FName("Bold");
    
    //const UEquipmentSettings* Settings = UEquipmentSettings::Get();

    //SlotOptions.Reset();
    //for (const FEquipmentCategoryData& CData : Settings->EquipmentCategories)
    //{
    //    const TArray<FEquipmentSlotData>& SlotData = CData.Slots;
    //    for (const FEquipmentSlotData& SData : SlotData)
    //    {
    //        TSharedPtr<FEquipmentSlotId> Option = MakeShared<FEquipmentSlotId>();
    //        Option->Id = SData.SlotId;
    //        Option->SlotTag = CData.CategoryTag;

    //        SlotOptions.Add(MoveTemp(Option));
    //    }
    //}

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
                .Padding(FMargin(0.0f, 4.0f))
                [
                    SNew(SGridPanel)
                        + SGridPanel::Slot(0, 0)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(SlotTag_Property->GetPropertyDisplayName())
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 0)
                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                        [
                            SNew(SGameplayTagCombo)
                                .PropertyHandle(SlotTag_Property)
                        ]
                        + SGridPanel::Slot(0, 1)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                                .Text(Id_Property->GetPropertyDisplayName())
                                .Font(BoldFont)
                        ]
                        + SGridPanel::Slot(1, 1)
                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                        [
                            Id_Property->CreatePropertyValueWidget()
                        ]
                ]
                //+ SVerticalBox::Slot()
                //.Padding(FMargin(0.0f, 4.0f))
                //.AutoHeight()
                //    [
                //        SNew(SComboBox<TSharedPtr<FEquipmentSlotId>>)
                //            .OptionsSource(&SlotOptions)
                //            .OnGenerateWidget_Lambda(
                //                [CustomFont, BoldFont](TSharedPtr<FEquipmentSlotId> Item)
                //                {
                //                    return SNew(SGridPanel)
                //                        + SGridPanel::Slot(0, 0)
                //                        .VAlign(VAlign_Center)
                //                        [
                //                            SNew(STextBlock)
                //                                .Text(FText::FromString("Tag:"))
                //                                .Font(BoldFont)
                //                        ]
                //                        + SGridPanel::Slot(1, 0)
                //                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                //                        [
                //                            SNew(STextBlock)
                //                                .Text(FText::FromString(Item->SlotTag.ToString()))
                //                                .Font(CustomFont)
                //                        ]
                //                        + SGridPanel::Slot(0, 1)
                //                        .VAlign(VAlign_Center)
                //                        [
                //                            SNew(STextBlock)
                //                                .Text(FText::FromString("Id:"))
                //                                .Font(BoldFont)
                //                        ]
                //                        + SGridPanel::Slot(1, 1)
                //                        .Padding(6.0f, 0.0f, 0.0f, 0.0f)
                //                        [
                //                            SNew(STextBlock)
                //                                .Text(FText::FromString(FString::FromInt(Item->Id)))
                //                                .Font(CustomFont)
                //                        ];
                //                })
                //            .OnSelectionChanged_Lambda(
                //                [this](TSharedPtr<FEquipmentSlotId> NewSelection, ESelectInfo::Type SelectInfo)
                //                {
                //                    if (!NewSelection.IsValid())
                //                    {
                //                        return;
                //                    }

                //                    SlotTag_Property->SetValue(NewSelection->SlotTag.ToString());
                //                    Id_Property->SetValue(NewSelection->Id);
                //                })
                //            [
                //                SNew(STextBlock)
                //                    .Text_Lambda([this]()
                //                        {
                //                            int32 CurrentId = INDEX_NONE;

                //                            FGameplayTag CurrentTag;
                //                            void* CurrentTagPtr = nullptr;

                //                            if (Id_Property->GetValue(CurrentId) != FPropertyAccess::Success)
                //                            {
                //                                return FText::FromString(TEXT("Select Slot"));
                //                            }

                //                            if (SlotTag_Property->GetValueData(CurrentTagPtr) == FPropertyAccess::Success)
                //                            {
                //                                CurrentTag = *static_cast<FGameplayTag*>(CurrentTagPtr);
                //                            }
                //                            else
                //                            {
                //                                return FText::FromString(TEXT("Select Slot"));
                //                            }

                //                            for (const TSharedPtr<FEquipmentSlotId>& Option : SlotOptions)
                //                            {
                //                                if (Option.IsValid() && (Option->Id == CurrentId && Option->SlotTag == CurrentTag))
                //                                {
                //                                    FString DisplayText = Option->SlotTag.ToString() + " @ " + FString::FromInt(Option->Id);

                //                                    return FText::FromString(DisplayText);
                //                                }
                //                            }

                //                            return FText::FromString(TEXT("Select Slot"));
                //                        })
                //            ]
                //    ]
        ];
}

void FEquipmentSlotIdCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    
}


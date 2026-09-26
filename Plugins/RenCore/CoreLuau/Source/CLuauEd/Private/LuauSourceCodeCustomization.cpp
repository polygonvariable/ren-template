// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "LuauSourceCodeCustomization.h"

// Engine Headers
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "IDetailChildrenBuilder.h"

// Project Headers
#include "LuauFragment.h"
#include "LuauSyntaxHighlighter.h"


TSharedRef<IPropertyTypeCustomization> FLuauSourceCodeCustomization::MakeInstance()
{
    return MakeShared<FLuauSourceCodeCustomization>();
}

void FLuauSourceCodeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    HeaderRow.NameContent()
        [
            StructPropertyHandle->CreatePropertyNameWidget()
        ];

    HeaderRow.ValueContent()
        [
            StructPropertyHandle->CreatePropertyValueWidget()
        ];
}

void FLuauSourceCodeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
    ParentHandle = StructPropertyHandle;

    CodeProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FLuauSourceCode, Code));
    BytecodeProperty = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FLuauSourceCode, Bytecode));
    BytecodeArrayProperty = BytecodeProperty->AsArray();
    
    float Padding = 6.0;

    FMargin PaddingAll = FMargin(Padding);
    FMargin PaddingV = FMargin(Padding, 0.0f, Padding, Padding);
    FMargin PaddingH = FMargin(0.0f, 0.0f, Padding, 0.0f);

    TSharedPtr<FLuauSyntaxHighlighter> SyntaxHighlighter = FLuauSyntaxHighlighter::Create();

    ChildBuilder.AddCustomRow(FText::FromString(TEXT("Text")))
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(PaddingAll)
                [
                    SNew(STextBlock)
                        .Text(CodeProperty->GetPropertyDisplayName())
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(PaddingV)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(PaddingH)
                        [
                            SNew(SButton)
                                .OnClicked(this, &FLuauSourceCodeCustomization::CompileCode)
                                .Text(FText::FromString(TEXT("Compile")))
                        ]
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(PaddingH)
                        [
                            SNew(SButton)
                                .OnClicked(this, &FLuauSourceCodeCustomization::ClearCode)
                                .Text(FText::FromString(TEXT("Clear")))
                        ]
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(PaddingV)
                [
                    SNew(SMultiLineEditableTextBox)
                        .Text_Lambda([this]()
                            {
                                FString Value;
                                CodeProperty->GetValue(Value);

                                return FText::FromString(Value);
                            })
                        .OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type)
                            {
                                CodeProperty->SetValue(NewText.ToString());
                            })
                        .Marshaller(SyntaxHighlighter)
                ]
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(PaddingV)
                [
                    SNew(STextBlock)
                        .Text_Lambda([this]()
                            {
                                if (!BytecodeArrayProperty.IsValid())
                                {
                                    return FText::FromString(TEXT("Bytecode size: 0"));
                                }

                                uint32 Num = 0;
                                BytecodeArrayProperty->GetNumElements(Num);

                                return FText::Format(FText::FromString(TEXT("Bytecode size: {0}")), FText::AsNumber(Num));
                            })
                ]
        ];
}

FReply FLuauSourceCodeCustomization::CompileCode()
{
    FLuauSourceCode* Struct = nullptr;
    if (!GetParent(Struct))
    {
        return FReply::Handled();
    }

    ParentHandle->NotifyPreChange();

    FString& Code = Struct->Code;
    TArray<uint8>& Bytecode = Struct->Bytecode;

    if (FLuauHelper::Compile(Code, Bytecode))
    {
        UE_LOG(LogTemp, Warning, TEXT("Code compiled"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to compile code"));
    }

    ParentHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    ParentHandle->NotifyFinishedChangingProperties();

    return FReply::Handled();
}

FReply FLuauSourceCodeCustomization::ClearCode()
{
    FLuauSourceCode* Struct = nullptr;
    if (!GetParent(Struct))
    {
        return FReply::Handled();
    }

    ParentHandle->NotifyPreChange();

    Struct->Bytecode.Empty();

    ParentHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    ParentHandle->NotifyFinishedChangingProperties();

    return FReply::Handled();
}

bool FLuauSourceCodeCustomization::GetParent(FLuauSourceCode*& Struct) const
{
    if (ParentHandle->GetValueData((void*&)Struct) != FPropertyAccess::Success || Struct == nullptr)
    {
        return false;
    }
    return true;
}


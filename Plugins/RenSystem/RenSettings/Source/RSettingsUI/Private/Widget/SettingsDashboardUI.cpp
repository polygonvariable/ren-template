// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/SettingsDashboardUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"


void USettingOptionUI::NativePreConstruct()
{
	Super::NativePreConstruct();
	TitleText->SetText(Title);
}

void USettingOptionUI::NativeConstruct()
{
	Super::NativeConstruct();
	LoadCVar();
}

void USettingOptionUI::NativeDestruct()
{
	_CVar = nullptr;
	Super::NativeDestruct();
}


bool USettingOptionUI::GetSettingValue(bool& Value) const { return false; }
bool USettingOptionUI::GetSettingValue(int& Value) const { return false; }
bool USettingOptionUI::GetSettingValue(float& Value) const { return false; }

void USettingOptionUI::SetSettingValue(bool Value) {}
void USettingOptionUI::SetSettingValue(int Value) {}
void USettingOptionUI::SetSettingValue(float Value) {}

void USettingOptionUI::LoadCVar()
{
	if (!TargetCVar.IsEmpty())
	{
		_CVar = IConsoleManager::Get().FindConsoleVariable(*TargetCVar);
		if (_CVar)
		{
			switch (ValueType)
			{
			case ESettingValueType::Bool:
				SetSettingValue(_CVar->GetBool());
				break;
			case ESettingValueType::Int:
				SetSettingValue(_CVar->GetInt());
				break;
			case ESettingValueType::Float:
				SetSettingValue(_CVar->GetFloat());
				break;
			}
		}
	}
}

void USettingOptionUI::SaveCVar()
{
	if (_CVar)
	{
		EConsoleVariableFlags Flags = bHighestPriority ? ECVF_SetByConsole : ECVF_SetByGameSetting;

		switch (ValueType)
		{
		case ESettingValueType::Bool:
			bool bValue;
			if (GetSettingValue(bValue))
			{
				_CVar->Set(bValue, Flags);
			}
			break;
		case ESettingValueType::Int:
			int iValue;
			if (GetSettingValue(iValue))
			{
				_CVar->Set(iValue, Flags);
			}
			break;
		case ESettingValueType::Float:
			float fValue;
			if (GetSettingValue(fValue))
			{
				_CVar->Set(fValue, Flags);
			}
			break;
		}
	}
}


void USettingOption_DropdownUI::HandleOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bAutoSave)
	{
		SaveCVar();
	}
}

bool USettingOption_DropdownUI::GetSettingValue(bool& Value) const
{
	int Index = SettingDropdown->GetSelectedIndex();
	if (!Options.IsValidIndex(Index))
	{
		return false;
	}

	Value = Options[Index].BoolOption;
	return true;
}

bool USettingOption_DropdownUI::GetSettingValue(int& Value) const
{
	int Index = SettingDropdown->GetSelectedIndex();
	if (!Options.IsValidIndex(Index))
	{
		return 0;
	}

	Value = Options[Index].IntOption;
	return true;
}


void USettingOption_DropdownUI::SetSettingValue(int Value)
{
	int Index = Options.IndexOfByPredicate([Value](const FSettingDropdownOption& Option) { return Option.IntOption == Value; });
	SettingDropdown->SetSelectedIndex(Index);
}

void USettingOption_DropdownUI::SetSettingValue(bool Value)
{
	int Index = Options.IndexOfByPredicate([Value](const FSettingDropdownOption& Option) { return Option.BoolOption == Value; });
	SettingDropdown->SetSelectedIndex(Index);
}

void USettingOption_DropdownUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	for (const FSettingDropdownOption& Item : Options)
	{
		SettingDropdown->AddOption(Item.Title);
	}
}

void USettingOption_DropdownUI::NativeConstruct()
{
	SettingDropdown->OnSelectionChanged.AddDynamic(this, &USettingOption_DropdownUI::HandleOnSelectionChanged);
	Super::NativeConstruct();
}

void USettingOption_DropdownUI::NativeDestruct()
{
	SettingDropdown->OnSelectionChanged.Clear();
	Super::NativeDestruct();
}


USettingOption_SliderUI::USettingOption_SliderUI()
{
	ValueType = ESettingValueType::Int;
}

void USettingOption_SliderUI::HandleOnValueChanged(float Value)
{
	if (bAutoSave)
	{
		SaveCVar();
	}
	HandleValueText(Value);
}

void USettingOption_SliderUI::HandleValueText(float Value)
{
	if (OptionText.IsValidIndex(Value))
	{
		ValueText->SetText(OptionText[Value]);
	}
	else
	{
		ValueText->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

bool USettingOption_SliderUI::GetSettingValue(int& Value) const
{
	Value = SettingSlider->GetValue();
	return true;
}

void USettingOption_SliderUI::SetSettingValue(int Value)
{
	SettingSlider->SetValue(Value);
}

void USettingOption_SliderUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	SettingSlider->SetMinValue(MinValue);
	SettingSlider->SetMaxValue(MaxValue);
	SettingSlider->SetValue(DefaultValue);

	HandleValueText(DefaultValue);
}

void USettingOption_SliderUI::NativeConstruct()
{
	SettingSlider->OnValueChanged.AddDynamic(this, &USettingOption_SliderUI::HandleOnValueChanged);
	Super::NativeConstruct();
}

void USettingOption_SliderUI::NativeDestruct()
{
	SettingSlider->OnValueChanged.Clear();
	Super::NativeDestruct();
}


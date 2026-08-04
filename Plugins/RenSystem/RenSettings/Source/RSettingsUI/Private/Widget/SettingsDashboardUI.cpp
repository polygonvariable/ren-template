// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/SettingsDashboardUI.h"

// Engine Headers
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/Button.h"
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

int USettingOptionUI::GetSettingOptionValue() const
{
	return 0;
}

void USettingOptionUI::SetSettingOptionValue(int Value)
{
}

void USettingOptionUI::LoadCVar()
{
	if (!TargetCVar.IsEmpty())
	{
		_CVar = IConsoleManager::Get().FindConsoleVariable(*TargetCVar);
		if (_CVar)
		{
			SetSettingOptionValue(_CVar->GetInt());
		}
	}
}

void USettingOptionUI::SaveCVar()
{
	if (_CVar)
	{
		if (bHighestPriority)
		{
			_CVar->Set(GetSettingOptionValue(), ECVF_SetByConsole);
		}
		else
		{
			_CVar->Set(GetSettingOptionValue(), ECVF_SetByGameSetting);
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

int USettingOption_DropdownUI::GetSettingOptionValue() const
{
	int Index = SettingDropdown->GetSelectedIndex();
	if (!bUseValueOption)
	{
		return Index;
	}

	if (ValueOptions.IsValidIndex(Index))
	{
		return ValueOptions[Index];
	}

	return 0;
}

void USettingOption_DropdownUI::SetSettingOptionValue(int Value)
{
	SettingDropdown->SetSelectedIndex(Value);
}

void USettingOption_DropdownUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	for (const FString& Option : DefaultOptions)
	{
		SettingDropdown->AddOption(Option);
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

int USettingOption_SliderUI::GetSettingOptionValue() const
{
	return SettingSlider->GetValue();
}

void USettingOption_SliderUI::SetSettingOptionValue(int Value)
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


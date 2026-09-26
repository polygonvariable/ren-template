// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "SettingsDashboardUI.generated.h"

// Forward Declarations
class UTextBlock;
class UProgressBar;
class UButton;
class UComboBoxString;
class USlider;
class UGameUserSettings;


/**
 *
 */
UENUM()
enum class ESettingValueType
{
	Bool,
	Int,
	Float
};


/**
 *
 */
UCLASS(Abstract)
class USettingOptionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	void LoadCVar();
	void SaveCVar();

protected:

	UPROPERTY(EditAnywhere)
	ESettingValueType ValueType;

	UPROPERTY(EditAnywhere)
	FString TargetCVar;

	UPROPERTY(EditAnywhere)
	FText Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText = nullptr;

	UPROPERTY()
	TObjectPtr<UGameUserSettings> GameSettings = nullptr;

	UPROPERTY(EditAnywhere)
	bool bAutoSave = true;

	UPROPERTY(EditAnywhere)
	bool bHighestPriority = false;


	virtual bool GetSettingValue(bool& Value) const;
	virtual bool GetSettingValue(int& Value) const;
	virtual bool GetSettingValue(float& Value) const;

	virtual void SetSettingValue(bool Value);
	virtual void SetSettingValue(int Value);
	virtual void SetSettingValue(float Value);


	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	IConsoleVariable* _CVar = nullptr;

};


/**
 *
 */
USTRUCT()
struct FSettingDropdownOption
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FString Title;

	UPROPERTY(EditAnywhere)
	int IntOption;

	UPROPERTY(EditAnywhere)
	bool BoolOption;

};


/**
 *
 */
UCLASS(Abstract)
class USettingOption_DropdownUI : public USettingOptionUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TArray<FSettingDropdownOption> Options;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> SettingDropdown = nullptr;


	// ~ Binding
	UFUNCTION()
	void HandleOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// ~ End of Binding

	// ~ USettingOptionUI
	virtual bool GetSettingValue(bool& Value) const override;
	virtual bool GetSettingValue(int& Value) const override;
	virtual void SetSettingValue(int Value) override;
	virtual void SetSettingValue(bool Value) override;
	// ~ End of USettingOptionUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 *
 */
UCLASS(Abstract)
class USettingOption_SliderUI : public USettingOptionUI
{

	GENERATED_BODY()

public:

	USettingOption_SliderUI();

protected:

	UPROPERTY(EditAnywhere)
	TArray<FText> OptionText;

	UPROPERTY(EditAnywhere)
	int DefaultValue = 0;

	UPROPERTY(EditAnywhere)
	int MinValue = 0;

	UPROPERTY(EditAnywhere)
	int MaxValue = 4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SettingSlider = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText = nullptr;


	// ~ Binding
	UFUNCTION()
	void HandleOnValueChanged(float Value);
	void HandleValueText(float Value);
	// ~ End of Binding

	// ~ USettingOptionUI
	virtual bool GetSettingValue(int& Value) const override;
	virtual void SetSettingValue(int Value) override;
	// ~ End of USettingOptionUI

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 *
 */
UCLASS(Abstract)
class USettingsDashboardUI : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(EditAnywhere)
	bool bHideOnClose = true;


	UFUNCTION()
	void CloseWidget();

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


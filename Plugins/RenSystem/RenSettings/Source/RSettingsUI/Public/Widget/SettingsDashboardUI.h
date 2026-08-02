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
UCLASS(Abstract)
class USettingOptionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	void LoadCVar();
	void SaveCVar();

protected:

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


	virtual int GetSettingOptionValue() const;
	virtual void SetSettingOptionValue(int Value);


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
UCLASS(Abstract)
class USettingOption_DropdownUI : public USettingOptionUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	TArray<FString> DefaultOptions;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> SettingDropdown = nullptr;

	// ~ Binding
	UFUNCTION()
	void HandleOnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	// ~ End of Binding

	// ~ USettingOptionUI
	virtual int GetSettingOptionValue() const override;
	virtual void SetSettingOptionValue(int Value) override;
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
	virtual int GetSettingOptionValue() const override;
	virtual void SetSettingOptionValue(int Value) override;
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
class USettingOption_ToggleUI : public USettingOptionUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	bool bDefaultValue = false;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SettingToggle = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText = nullptr;


	// ~ Binding
	UFUNCTION()
	void HandleOnButtonClicked();
	void HandleValueText(bool bState);
	// ~ End of Binding

	// ~ USettingOptionUI
	virtual int GetSettingOptionValue() const override;
	virtual void SetSettingOptionValue(int Value) override;
	// ~ End of USettingOptionUI
	
	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	bool _bCurrentState = false;

};


/**
 *
 */
UCLASS(Abstract)
class USettingsDashboardUI : public UUserWidget
{

	GENERATED_BODY()

};


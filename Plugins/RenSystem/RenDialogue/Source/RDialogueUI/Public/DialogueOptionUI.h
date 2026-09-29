// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "DialogueOptionUI.generated.h"

// Declare Delegates
DECLARE_DELEGATE_OneParam(FDialogueSelectDelegate, int /* Index */);

// Forward Declarations
class UButton;
class UTextBlock;
class UOverlay;
class UPanelWidget;


/**
 * 
 */
UCLASS(Abstract)
class UDialogueOptionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	FDialogueSelectDelegate OnSelected;


	int GetOption() const;
	void SetOption(int Index, const FText& FText);
	void ClearOption();

	bool IsActive() const;
	void SetActive(bool bActive);

protected:

	bool bIsActive = false;
	int OptionIndex = 0;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OptionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UOverlay> ActiveHighlight = nullptr;


	// ~ Binding
	UFUNCTION()
	void HandleOnOptionSelected();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 *
 */
UCLASS(Abstract)
class UDialogueOptionCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	FDialogueSelectDelegate OnSelected;


	void SetOptions(const TArray<FText>& Options);
	void ClearOptions();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> OptionsPanel = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<UDialogueOptionUI> OptionUIClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	FMargin OptionMargin = FMargin(0.0f, 8.0f, 0.0f, 0.0f);


	// ~ Binding
	void HandleOnOptionSelected(int Index);
	// ~ End of Binding

private:

	TArray<TObjectPtr<UDialogueOptionUI>> OptionsPool;

};


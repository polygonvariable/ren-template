// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Generated Headers
#include "DialoguePromptUI.generated.h"

// Forward Declarations
class UButton;
class UTextBlock;


/**
 *
 */
UCLASS(Abstract)
class UDialoguePromptUI : public UUserWidget
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FOnPromptedResult, bool);
	FOnPromptedResult OnPrompted;


	void SetSummary(const FText& Summary);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SummaryText = nullptr;


	// ~ Binding
	UFUNCTION()
	void HandleOnConfirmClicked();

	UFUNCTION()
	void HandleOnCancelClicked();
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


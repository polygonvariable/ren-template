// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "DialogueWidget.generated.h"

// Forward Declarations
class UButton;
class UTextBlock;
class UVerticalBox;

class UEventflowNode;
class UDialogueAsset;



UCLASS(Abstract)
class UDialogueOptionWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void InitializeDetails(FText Option, int Index);

protected:

	int OptionIndex = -1;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> OptionText;

	UFUNCTION(BlueprintCallable)
	void SelectOption();

public:

	DECLARE_DELEGATE_OneParam(FOnOptionSelected, int /* Index */);
	FOnOptionSelected OnOptionSelected;

};



UCLASS(Abstract)
class UDialogueWidget : public UUserWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UButton> NextButton;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DialogueTitle;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> DialogueContent;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> DialogueOptions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDialogueAsset> DialogueAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDialogueOptionWidget> OptionWidgetClass;

	UPROPERTY()
	TObjectPtr<UEventflowNode> CurrentNode;


	UFUNCTION(BlueprintCallable)
	void ShowDialogue();

	void ShowOptions();

	void NextDialogue();

	void TrySelectOption(int Index);

	void SetCurrentNode(UEventflowNode* Node);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


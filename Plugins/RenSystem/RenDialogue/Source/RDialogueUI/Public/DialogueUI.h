// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Blueprint/UserWidget.h"

// Project Headers
#include "DialogueData.h"

// Generated Headers
#include "DialogueUI.generated.h"

// Forward Declarations
class UButton;
class UTextBlock;
class UPanelWidget;
class UDialogueAsset;
class UDialogueEngine;
class UDialogueOptionCollectionUI;
class UDialoguePromptUI;
class UDialogueSpeakerUI;


/**
 *
 */
UCLASS(Abstract)
class UDialogueUI : public UUserWidget
{

	GENERATED_BODY()

public:

	void InitializeDialogue(const UDialogueAsset* InAsset, UDialogueEngine* InEngine);
	void DeinitializeDialogue();

#if WITH_EDITOR
	// ~ UUserWidget
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UUserWidget
#endif

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeakerName = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDialogueOptionCollectionUI> DialogueOption = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkipButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> SpeakersPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDialoguePromptUI> DialoguePrompt = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TSubclassOf<UDialogueSpeakerUI> SpeakerUIClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	float ActionEnableTime = 1.0f;


	void SetSpeakers(const TArray<FDialogueSpeaker>& Speakers);
	void SetActiveSpeaker(const FDialogueSpeaker& Speaker);
	void ClearSpeakers();

	void SetSummary(const FText& Summary);

	void SetDialogue(const FDialogueData& Dialogue, const FDialogueSpeaker& Speaker);
	void ClearDialogue();

	void SetOptions(const TArray<FText>& Options);
	void ClearOptions();

	// ~ Binding
	UFUNCTION()
	void HandleOnNextClicked();

	UFUNCTION()
	void HandleOnSkipClicked();

	void HandleOnSkipPrompt(bool bResult);
	void HandleOnOptionClicked(int Index);
	// ~ End of Binding

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	UPROPERTY()
	TWeakObjectPtr<UDialogueEngine> _Engine;

};


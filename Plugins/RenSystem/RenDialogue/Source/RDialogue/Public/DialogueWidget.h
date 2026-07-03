// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "DialogueNodeData.h"

// Generated Headers
#include "DialogueWidget.generated.h"

// Forward Declarations
class UImage;
class UButton;
class UTextBlock;
class UPanelWidget;
class UVerticalBox;
class UDialogueAsset;



DECLARE_DELEGATE_OneParam(FOnDialogueCompleted, int /* Next Index */);


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UDialogueProvider : public UInterface
{
	GENERATED_BODY()
};

class IDialogueProvider
{

	GENERATED_BODY()

public:


	virtual void InitializeDialogue(const FDialogueData* Dialogue) = 0;
	virtual void ClearDialogue() = 0;

	virtual FOnDialogueCompleted& GetOnDialogueCompleted() = 0;

};



/**
 * 
 */
UCLASS(Abstract)
class UDialogueOptionWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FDialogueSelectDelegate, int /* Index */);
	FDialogueSelectDelegate OnDialogueSelect;

	void InitializeOption(int Index, const FText& FText);
	void ResetOption();

protected:

	int CurrentIndex = 0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OptionText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton = nullptr;

	UFUNCTION()
	void HandleSelectClicked();

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 * 
 */
UCLASS(Abstract)
class UDialogueWidget : public UUserWidget, public IDialogueProvider
{

	GENERATED_BODY()

public:

	// ~ IDialogueProvider
	virtual void InitializeDialogue(const FDialogueData* Dialogue) override;
	virtual void ClearDialogue() override;
	virtual FOnDialogueCompleted& GetOnDialogueCompleted() override;
	// ~ End of IDialogueProvider

protected:

	FOnDialogueCompleted OnDialogueCompleted;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueOptionWidget> OptionWidgetClass = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SpeakerImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SpeakerName = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> OptionPanel = nullptr;


	virtual void InitializeOption(const FDialogueData* Dialogue);
	virtual void ResetOption();

	UFUNCTION()
	void HandleNextClicked();

	UFUNCTION()
	void HandleOptionClicked(int Index);

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

private:

	TArray<TObjectPtr<UDialogueOptionWidget>> _OptionPool;

};


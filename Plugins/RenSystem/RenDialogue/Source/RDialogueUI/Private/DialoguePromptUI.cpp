// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialoguePromptUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UDialoguePromptUI::SetSummary(const FText& Summary)
{
	SummaryText->SetText(Summary);
}

void UDialoguePromptUI::HandleOnConfirmClicked()
{
	OnPrompted.ExecuteIfBound(true);
}

void UDialoguePromptUI::HandleOnCancelClicked()
{
	OnPrompted.ExecuteIfBound(false);
}

void UDialoguePromptUI::NativeConstruct()
{
	ConfirmButton->OnClicked.AddDynamic(this, &UDialoguePromptUI::HandleOnConfirmClicked);
	CancelButton->OnClicked.AddDynamic(this, &UDialoguePromptUI::HandleOnCancelClicked);

	Super::NativeConstruct();
}

void UDialoguePromptUI::NativeDestruct()
{
	ConfirmButton->OnClicked.RemoveAll(this);
	CancelButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}


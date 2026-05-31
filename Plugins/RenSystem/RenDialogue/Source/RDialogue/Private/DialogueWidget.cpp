// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueWidget.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Project Headers
#include "DialogueAsset.h"
#include "DialogueNodeData.h"
#include "EventflowAsset.h"
#include "EventflowEngine.h"
#include "Log/LogMacro.h"
#include "Library/PoolHelper.h"



void UDialogueOptionWidget::InitializeOption(int Index, const FText& FText)
{
	CurrentIndex = Index;
	OptionText->SetText(FText);
}

void UDialogueOptionWidget::ResetOption()
{
	CurrentIndex = -1;
	OptionText->SetText(FText::GetEmpty());
}

void UDialogueOptionWidget::HandleSelectClicked()
{
	OnDialogueSelect.ExecuteIfBound(CurrentIndex);
}

void UDialogueOptionWidget::NativeConstruct()
{
	SelectButton->OnClicked.AddDynamic(this, &UDialogueOptionWidget::HandleSelectClicked);
	Super::NativeConstruct();
}

void UDialogueOptionWidget::NativeDestruct()
{
	SelectButton->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}






void UDialogueWidget::InitializeDialogue(const FDialogueData* Dialogue)
{
	ClearDialogue();

	if (Dialogue)
	{
		SpeakerImage->SetBrushFromSoftTexture(Dialogue->SpeakerImage);
		SpeakerName->SetText(Dialogue->Speaker);
		DialogueText->SetText(Dialogue->Content);

		InitializeOption(Dialogue);
	}
}

void UDialogueWidget::ClearDialogue()
{
	ResetOption();
}

FOnDialogueCompleted& UDialogueWidget::GetOnDialogueCompleted()
{
	return OnDialogueCompleted;
}



void UDialogueWidget::InitializeOption(const FDialogueData* Dialogue)
{
	if (!IsValid(OptionWidgetClass))
	{
		LOG_ERROR(LogTemp, TEXT("OptionWidgetClass is invalid"));
		return;
	}
	
	const TArray<FText>& Options = Dialogue->Options;
	int Count = Options.Num();

	for (int i = 0; i < Count; i++)
	{
		UDialogueOptionWidget* OptionWidget = FPoolHelper::AcquireWidgetFromArray<UDialogueOptionWidget>(_OptionPool, OptionWidgetClass, this);
		if (IsValid(OptionWidget))
		{
			OptionWidget->InitializeOption(i, Options[i]);
			OptionWidget->OnDialogueSelect.BindUObject(this, &UDialogueWidget::HandleOptionClicked);
			OptionPanel->AddChild(OptionWidget);
		}
	}

	if (Count > 0)
	{
		OptionPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NextButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		OptionPanel->SetVisibility(ESlateVisibility::Collapsed);
		NextButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDialogueWidget::ResetOption()
{
	int Count = OptionPanel->GetChildrenCount();
	for (int i = 0; i < Count; i++)
	{
		UDialogueOptionWidget* OptionWidget = Cast<UDialogueOptionWidget>(OptionPanel->GetChildAt(i));
		if (IsValid(OptionWidget))
		{
			OptionWidget->OnDialogueSelect.Unbind();
			OptionWidget->ResetOption();
			FPoolHelper::ReturnToArray(_OptionPool, OptionWidget);
		}
	}
	OptionPanel->ClearChildren();
}




void UDialogueWidget::HandleNextClicked()
{
	OnDialogueCompleted.ExecuteIfBound(0);
}

void UDialogueWidget::HandleOptionClicked(int Index)
{
	OnDialogueCompleted.ExecuteIfBound(Index);
}

void UDialogueWidget::NativeConstruct()
{
	NextButton->OnClicked.AddDynamic(this, &UDialogueWidget::HandleNextClicked);

	Super::NativeConstruct();
}

void UDialogueWidget::NativeDestruct()
{
	NextButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}


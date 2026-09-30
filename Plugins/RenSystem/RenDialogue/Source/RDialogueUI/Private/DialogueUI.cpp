// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueUI.h"

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

// Project Headers
#include "DialogueAsset.h"
#include "DialogueData.h"
#include "DialogueEngine.h"
#include "DialogueOptionUI.h"
#include "DialoguePromptUI.h"
#include "DialogueSpeakerUI.h"


void UDialogueUI::InitializeDialogue(const UDialogueAsset* InAsset, UDialogueEngine* InEngine)
{
	_Engine = TWeakObjectPtr<UDialogueEngine>(InEngine);

	SetSummary(InAsset->Summary);
	SetSpeakers(InAsset->Speakers);

	if (IsValid(InEngine))
	{
		InEngine->OnDialogueContentUpdated.BindWeakLambda(this,
			[this](const FDialogueData& Dialogue, const FDialogueSpeaker& Speaker)
			{
				SetDialogue(Dialogue, Speaker);
			}
		);
		InEngine->OnDialogueOptionsUpdated.BindWeakLambda(this,
			[this](const TArray<FText>& Options)
			{
				SetOptions(Options);
			}
		);
		InEngine->OnDialogueRemoved.BindWeakLambda(this,
			[this]()
			{
				ClearDialogue();
				ClearOptions();
			}
		);
	}
}

void UDialogueUI::DeinitializeDialogue()
{
	UDialogueEngine* Engine = _Engine.Get();
	if (IsValid(Engine))
	{
		Engine->OnDialogueContentUpdated.Unbind();
		Engine->OnDialogueOptionsUpdated.Unbind();
		Engine->OnDialogueRemoved.Unbind();
	}

	ClearDialogue();
	ClearOptions();
	ClearSpeakers();
	SetSummary(FText::GetEmpty());

	_Engine.Reset();
}


#if WITH_EDITOR
EDataValidationResult UDialogueUI::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (!IsValid(SpeakerUIClass))
	{
		Context.AddError(FText::FromString("Speaker widget class is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (ActionEnableTime <= 0.01f || ActionEnableTime >= 10.0f)
	{
		Context.AddError(FText::FromString("Enable timer is outof valid range"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif


void UDialogueUI::SetSpeakers(const TArray<FDialogueSpeaker>& Speakers)
{
	if (!IsValid(SpeakerUIClass))
	{
		return;
	}

	for (const FDialogueSpeaker& Speaker : Speakers)
	{
		UDialogueSpeakerUI* Widget = CreateWidget<UDialogueSpeakerUI>(GetWorld(), SpeakerUIClass);
		Widget->Speaker = Speaker;
		SpeakersPanel->AddChild(Widget);
	}
}

void UDialogueUI::SetActiveSpeaker(const FDialogueSpeaker& Speaker)
{
	TArray<UWidget*> Children = SpeakersPanel->GetAllChildren();
	for (UWidget* Widget : Children)
	{
		UDialogueSpeakerUI* SpeakerWidget = Cast<UDialogueSpeakerUI>(Widget);
		if (IsValid(SpeakerWidget))
		{
			if (SpeakerWidget->Speaker.Name.EqualTo(Speaker.Name))
			{
				SpeakerWidget->SetImage(Speaker.Image);
				SpeakerWidget->SetActive(true);
			}
			else
			{
				SpeakerWidget->SetActive(false);
			}
		}
	}
}

void UDialogueUI::ClearSpeakers()
{
	SpeakersPanel->ClearChildren();
}


void UDialogueUI::SetSummary(const FText& Summary)
{
	DialoguePrompt->SetSummary(Summary);
}


void UDialogueUI::SetDialogue(const FDialogueData& Dialogue, const FDialogueSpeaker& Speaker)
{
	SpeakerName->SetText(Speaker.Name);
	DialogueText->SetText(Dialogue.Content);
	SetActiveSpeaker(Speaker);
}

void UDialogueUI::ClearDialogue()
{
	SpeakerName->SetText(FText::GetEmpty());
	DialogueText->SetText(FText::GetEmpty());
}


void UDialogueUI::SetOptions(const TArray<FText>& Options)
{
	DialogueOption->SetOptions(Options);
	if (Options.Num() > 0)
	{
		NextButton->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDialogueUI::ClearOptions()
{
	DialogueOption->ClearOptions();
	NextButton->SetVisibility(ESlateVisibility::Visible);
}


void UDialogueUI::HandleOnNextClicked()
{
	UDialogueEngine* Engine = _Engine.Get();
	if (IsValid(Engine))
	{
		Engine->NextDialogue(0);
	}
}

void UDialogueUI::HandleOnSkipClicked()
{
	DialoguePrompt->SetVisibility(ESlateVisibility::Visible);
}

void UDialogueUI::HandleOnSkipPrompt(bool bResult)
{
	if (bResult)
	{
		UDialogueEngine* Engine = _Engine.Get();
		if (IsValid(Engine))
		{
			Engine->SkipDialogue();
		}
	}
	else
	{
		DialoguePrompt->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UDialogueUI::HandleOnOptionClicked(int Index)
{
	UDialogueEngine* Engine = _Engine.Get();
	if (IsValid(Engine))
	{
		Engine->NextDialogue(Index);
	}
}


FReply UDialogueUI::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	//if (InKeyEvent.GetKey() == NextKey)
	//{
	//	HandleOnNextClicked();
	//	return FReply::Handled();
	//}
	//else if (InKeyEvent.GetKey() == SkipKey)
	//{
	//	HandleOnSkipClicked();
	//	return FReply::Handled();
	//}
	return FReply::Unhandled();
}

void UDialogueUI::NativeConstruct()
{
	DialoguePrompt->SetVisibility(ESlateVisibility::Collapsed);
	DialoguePrompt->OnPrompted.BindUObject(this, &UDialogueUI::HandleOnSkipPrompt);
	DialogueOption->OnSelected.BindUObject(this, &UDialogueUI::HandleOnOptionClicked);
	NextButton->OnClicked.AddDynamic(this, &UDialogueUI::HandleOnNextClicked);
	SkipButton->OnClicked.AddDynamic(this, &UDialogueUI::HandleOnSkipClicked);

	Super::NativeConstruct();
}

void UDialogueUI::NativeDestruct()
{
	DialoguePrompt->OnPrompted.Unbind();
	DialogueOption->OnSelected.Unbind();
	NextButton->OnClicked.RemoveAll(this);
	SkipButton->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}


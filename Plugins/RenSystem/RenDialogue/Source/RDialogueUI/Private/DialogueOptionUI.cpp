// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueOptionUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Core/PoolLibrary.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"


int UDialogueOptionUI::GetOption() const
{
	return OptionIndex;
}

void UDialogueOptionUI::SetOption(int Index, const FText& FText)
{
	OptionIndex = Index;
	OptionText->SetText(FText);
}

void UDialogueOptionUI::ClearOption()
{
	OptionIndex = -1;
	OptionText->SetText(FText::GetEmpty());
	SetActive(false);
}


bool UDialogueOptionUI::IsActive() const
{
	return bIsActive;
}

void UDialogueOptionUI::SetActive(bool bActive)
{
	//bIsActive = bActive;
	//if (bActive)
	//{
	//	ActiveHighlight->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}
	//else
	//{
	//	ActiveHighlight->SetVisibility(ESlateVisibility::Hidden);
	//}
}


void UDialogueOptionUI::HandleOnOptionSelected()
{
	OnSelected.ExecuteIfBound(OptionIndex);
}

void UDialogueOptionUI::NativeConstruct()
{
	OptionButton->OnClicked.AddDynamic(this, &UDialogueOptionUI::HandleOnOptionSelected);
	Super::NativeConstruct();
}

void UDialogueOptionUI::NativeDestruct()
{
	OptionButton->OnClicked.RemoveAll(this);
	Super::NativeDestruct();
}


void UDialogueOptionCollectionUI::SetOptions(const TArray<FText>& Options)
{
	if (!IsValid(OptionUIClass))
	{
		LOG_ERROR(LogDialogue, TEXT("OptionWidgetClass is invalid"));
		return;
	}

	int Num = Options.Num();
	for (int i = 0; i < Num; i++)
	{
		UDialogueOptionUI* OptionWidget = FPoolLibrary::AcquireWidgetFromArray<UDialogueOptionUI>(OptionsPool, OptionUIClass, this);
		if (IsValid(OptionWidget))
		{
			OptionWidget->SetActive(false);
			OptionWidget->SetOption(i, Options[i]);
			OptionWidget->SetPadding(OptionMargin);
			OptionWidget->OnSelected.BindUObject(this, &UDialogueOptionCollectionUI::HandleOnOptionSelected);
			OptionsPanel->AddChild(OptionWidget);
		}
	}

	if (Num > 0)
	{
		UDialogueOptionUI* OptionWidget = Cast<UDialogueOptionUI>(OptionsPanel->GetChildAt(0));
		if (IsValid(OptionWidget))
		{
			OptionWidget->SetActive(true);
		}
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UDialogueOptionCollectionUI::ClearOptions()
{
	int Num = OptionsPanel->GetChildrenCount();
	for (int i = 0; i < Num; i++)
	{
		UDialogueOptionUI* OptionWidget = Cast<UDialogueOptionUI>(OptionsPanel->GetChildAt(i));
		if (IsValid(OptionWidget))
		{
			OptionWidget->OnSelected.Unbind();
			OptionWidget->ClearOption();
			FPoolLibrary::ReturnToArray(OptionsPool, OptionWidget);
		}
	}

	OptionsPanel->ClearChildren();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UDialogueOptionCollectionUI::HandleOnOptionSelected(int Index)
{
	OnSelected.ExecuteIfBound(Index);
}


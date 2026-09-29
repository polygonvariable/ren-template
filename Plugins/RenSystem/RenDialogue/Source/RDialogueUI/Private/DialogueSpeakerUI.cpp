// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "DialogueSpeakerUI.h"

// Engine Headers
#include "Components/Image.h"

// Project Headers
#include "DialogueData.h"


void UDialogueSpeakerUI::SetActive(bool bActive)
{
	if (bActive)
	{
		SpeakerImage->SetBrushTintColor(FLinearColor(1.0f, 1.0f, 1.0f));
	}
	else
	{
		SpeakerImage->SetBrushTintColor(FLinearColor(0.2f, 0.2f, 0.2f));
	}
}

void UDialogueSpeakerUI::SetImage(TSoftObjectPtr<UTexture2D> Image)
{
	if (Image.IsNull())
	{
		SpeakerImage->SetBrushFromSoftTexture(Speaker.Image);
	}
	else
	{
		SpeakerImage->SetBrushFromSoftTexture(Image);
	}
}

void UDialogueSpeakerUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	SpeakerImage->SetBrushFromSoftTexture(Speaker.Image);
}


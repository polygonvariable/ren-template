// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/AvatarAsset.h"
#include "Widget/AvatarEntry.h"



void UAvatarEntryUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	const UAvatarAsset* AvatarAsset = Cast<UAvatarAsset>(Asset);
	if (!IsValid(AvatarAsset))
	{
		return;
	}

	EntryName->SetText(AvatarAsset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(AvatarAsset->Icon);
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AvatarEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Data/FragmentedDataAsset.h"


void UAvatarEntryUI::SetPrimaryDetail(const UFragmentedDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);

	SetToolTipText(Asset->DisplayName);
}


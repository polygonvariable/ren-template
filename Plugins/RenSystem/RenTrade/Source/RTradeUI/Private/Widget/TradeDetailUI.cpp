// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDetailUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Data/CoreDataAsset.h"



void UTradeDetailUI::ResetDetail()
{
	SwitchDetail(false);
}

void UTradeDetailUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		SwitchDetail(false);
		return;
	}

	SwitchDetail(true);

	EntryName->SetText(Asset->DisplayName);
	EntryDescription->SetText(Asset->Description);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}


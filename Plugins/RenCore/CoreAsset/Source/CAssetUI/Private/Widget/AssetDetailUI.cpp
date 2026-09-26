// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetDetailUI.h"

// Engine Headers
#include "Components/WidgetSwitcher.h"



void UAssetDetailUI::ResetDetail()
{
	SwitchDetail(false);
}

void UAssetDetailUI::SwitchDetail(bool bPrimary)
{
	if (IsValid(DetailSwitch))
	{
		DetailSwitch->SetActiveWidgetIndex(bPrimary ? 1 : 0);
	}
}


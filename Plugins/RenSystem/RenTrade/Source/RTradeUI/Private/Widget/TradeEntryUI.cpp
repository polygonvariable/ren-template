// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeEntryUI.h"

// Engine Headers
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Widget/TradeEntry.h"



void UTradeEntryUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	if (!IsValid(Asset))
	{
		return;
	}

	EntryName->SetText(Asset->DisplayName);
	EntryIcon->SetBrushFromSoftTexture(Asset->Icon);
}

void UTradeEntryUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UTradeEntry* ShopEntry = Cast<UTradeEntry>(Entry);
	if (!IsValid(ShopEntry))
	{
		return;
	}

	int Quantity = ShopEntry->TradeDetail.Quantity;
	int Quota = ShopEntry->TradeDetail.Quota;

	TradeQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	TradeQuota->SetText(FText::FromString(FString::FromInt(Quota)));
}


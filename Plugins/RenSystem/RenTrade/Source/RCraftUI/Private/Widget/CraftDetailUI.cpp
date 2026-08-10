// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftDetailUI.h"

// Engine Headers
#include "Components/TextBlock.h"

// Project Headers
#include "Core/Type/Runtime/TradeKey.h"
#include "System/CraftStorageManager.h"
#include "System/CraftSubsystem.h"
#include "Widget/CraftEntry.h"


void UCraftDetailUI::RefreshDetail()
{
	if (!IsValid(StorageManager))
	{
		SwitchDetail(false);
		return;
	}

	FTradeKey TradeKey(TradeAssetId, TradeCollectionId, GetActiveAssetId());
	const FCraftInstance* NewCraftData = StorageManager->GetItem(TradeKey);
	if (!NewCraftData)
	{
		SwitchDetail(false);
		return;
	}

	CraftData = *NewCraftData;

	RefreshCraftDetail();
}

void UCraftDetailUI::RefreshCraftDetail()
{
	CraftQuantity->SetText(FText::AsNumber(CraftData.GetCompletedQuantity()));

	FTimespan RemainingTime = CraftData.GetRemainingTime();
	if (RemainingTime.IsZero())
	{
		CraftTime->SetText(FText::FromString(TEXT("Completed")));
		return;
	}

	CraftTime->SetText(FText::AsNumber(RemainingTime.GetSeconds()));
}

void UCraftDetailUI::SetSecondaryDetail(const UAssetEntry* Entry)
{
	const UCraftEntry* CraftEntry = Cast<UCraftEntry>(Entry);
	if (!IsValid(CraftEntry))
	{
		SwitchDetail(false);
		return;
	}

	CraftData = CraftEntry->CraftData;

	RefreshCraftDetail();
}

void UCraftDetailUI::NativeConstruct()
{
	UCraftSubsystem* CraftSubsystem = UCraftSubsystem::Get(GetGameInstance());
	if (IsValid(CraftSubsystem))
	{
		StorageManager = CraftSubsystem->GetStorageManager();
		if (IsValid(StorageManager))
		{
			StorageManager->OnStorageUpdated.AddUObject(this, &UCraftDetailUI::RefreshDetail);
		}
	}

	Super::NativeConstruct();
}

void UCraftDetailUI::NativeDestruct()
{
	if (IsValid(StorageManager))
	{
		StorageManager->OnStorageUpdated.RemoveAll(this);
	}
	StorageManager = nullptr;

	Super::NativeDestruct();
}


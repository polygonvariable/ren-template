// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/CraftDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Subsystem/CraftSubsystem.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeEntry.h"



void UCraftBuildDashboardUI::HandleCraft()
{
	const UTradeEntry* Entry = PrimaryCollection->GetSelectedEntry<UTradeEntry>();
	if (!IsValid(Entry) || !IsValid(CraftSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Entry, CraftSubsystem is invalid"));
		return;
	}

	const FPrimaryAssetId& TargetAssetId = Entry->AssetId;

	FGuid TaskId = FGuid::NewGuid();
	CraftSubsystem->CraftItem(TaskId, TradeAssetId, TradeCollectionId, TargetAssetId, FTaskCallback::CreateWeakLambda(this, [](const FTaskResult& Result) {}));
}

const UAssetCollection* UCraftBuildDashboardUI::GetTradeMaterialCollection(const UCoreDataAsset* Asset) const
{
	if (!IsValid(CraftSubsystem))
	{
		return nullptr;
	}
	return CraftSubsystem->GetMaterialCollection(Asset, TradeCollectionId);
}

void UCraftBuildDashboardUI::NativeConstruct()
{
	CraftButton->OnClicked.AddDynamic(this, &UCraftBuildDashboardUI::HandleCraft);
	CraftSubsystem = UCraftSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UCraftBuildDashboardUI::NativeDestruct()
{
	CraftButton->OnClicked.RemoveAll(this);
	CraftSubsystem = nullptr;

	Super::NativeDestruct();
}



void UCraftClaimDashboardUI::HandleCraft()
{
	const UTradeEntry* Entry = PrimaryCollection->GetSelectedEntry<UTradeEntry>();
	if (!IsValid(Entry) || !IsValid(CraftSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Entry, CraftSubsystem is invalid"));
		return;
	}

	const FPrimaryAssetId& TargetAssetId = Entry->AssetId;

	FGuid TaskId = FGuid::NewGuid();
	CraftSubsystem->ClaimCraftItem(TaskId, TradeAssetId, TradeCollectionId, TargetAssetId, FTaskCallback::CreateWeakLambda(this, [](const FTaskResult& Result) {}));
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/ShopDashboardUI.h"

// Engine Headers
#include "Components/Button.h"

// Project Headers
#include "Asset/CoreDataAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Subsystem/ShopSubsystem.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeEntry.h"



void UShopDashboardUI::HandlePurchase()
{
	const UTradeEntry* Entry = PrimaryCollection->GetSelectedEntry<UTradeEntry>();
	if (!IsValid(Entry) || !IsValid(ShopSubsystem))
	{
		LOG_ERROR(LogShop, TEXT("Entry, ShopSubsystem is invalid"));
		return;
	}

	const FPrimaryAssetId& TargetAssetId = Entry->AssetId;

	FGuid TaskId = FGuid::NewGuid();
	ShopSubsystem->PurchaseItem(TaskId, TradeAssetId, TradeCollectionId, TargetAssetId, FTaskCallback::CreateWeakLambda(this, [](const FTaskResult& Result) {}));
}

const UAssetCollection* UShopDashboardUI::GetTradeMaterialCollection(const UCoreDataAsset* Asset) const
{
	if (!IsValid(ShopSubsystem))
	{
		return nullptr;
	}
	return ShopSubsystem->GetMaterialCollection(Asset, TradeCollectionId);
}

void UShopDashboardUI::NativeConstruct()
{
	ShopButton->OnClicked.AddDynamic(this, &UShopDashboardUI::HandlePurchase);
	ShopSubsystem = UShopSubsystem::Get(GetGameInstance());

	Super::NativeConstruct();
}

void UShopDashboardUI::NativeDestruct()
{
	ShopButton->OnClicked.RemoveAll(this);
	ShopSubsystem = nullptr;

	Super::NativeDestruct();
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDashboardUI.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Data/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Library/AssetManagerUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Widget/AssetCollectionUI.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeDetailUI.h"


void UTradeDashboardUI::InitializeDetail()
{
	if (!IsValid(AssetManager) || !TradeAssetId.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Entry, AssetManager or AssetId is invalid"));
		return;
	}

	FAssetManagerUtil::CancelHandle(_TradeHandle);

	_TradeHandle = AssetManager->LoadPrimaryAsset(TradeAssetId, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UTradeDashboardUI::InitializeTradeDetail));
}

void UTradeDashboardUI::ResetDetail()
{
	PrimaryDetail->ResetDetail();
	SecondaryCollection->ClearEntries(true);
}

const UAssetCollection* UTradeDashboardUI::GetTradeMaterialCollection(const UCoreDataAsset* Asset) const
{
	return nullptr;
}

void UTradeDashboardUI::InitializeTradeDetail()
{
	FAssetManagerUtil::ReleaseHandle(_TradeHandle);

	const UTradeAsset* Asset = AssetManager->GetPrimaryAssetObject<UTradeAsset>(TradeAssetId);
	if (!IsValid(Asset))
	{
		LOG_ERROR(LogAsset, TEXT("TradeAsset is invalid"));
		return;
	}

	PrimaryCollection->TradeAsset = Asset;
	PrimaryCollection->TradeCollectionId = TradeCollectionId;
	PrimaryCollection->PrimarySourceId = PrimarySourceId;
	PrimaryCollection->InitializeCollection();
	PrimaryCollection->DisplayEntries();

	PrimaryDetail->TradeAssetId = TradeAssetId;
	PrimaryDetail->TradeCollectionId = TradeCollectionId;
	PrimaryDetail->PrimarySourceId = PrimarySourceId;
	PrimaryDetail->InitializeDetail();

	SecondaryCollection->InitializeCollection();
}

void UTradeDashboardUI::SetPrimaryDetail(const UCoreDataAsset* Asset)
{
	UAssetEntry* Entry = PrimaryCollection->GetSelectedEntry();

	PrimaryDetail->InitializeAssetDetail(Asset);
	PrimaryDetail->InitializeEntryDetail(Entry);

	const UAssetCollection* MaterialCollection = GetTradeMaterialCollection(Asset);
	if (!IsValid(MaterialCollection))
	{
		return;
	}

	SecondaryCollection->ClearSubDetails();

	UFilterCriterion_Asset* AssetCriterion = SecondaryCollection->GetCriterionByName<UFilterCriterion_Asset>(FAssetFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		AssetCriterion->Included.Empty();

		TMap<FPrimaryAssetId, FAssetDetail> MaterialAssetList;
		MaterialCollection->GetAssetList(MaterialAssetList);

		for (const TPair<FPrimaryAssetId, FAssetDetail>& AssetKv : MaterialAssetList)
		{
			const FPrimaryAssetId& AssetId = AssetKv.Key;

			AssetCriterion->Included.Add(AssetId);
			SecondaryCollection->AddSubDetails(AssetId, FInstancedStruct::Make(AssetKv.Value));
		}
	}

	SecondaryCollection->RefreshEntries();
}

void UTradeDashboardUI::CancelInitialization()
{
	FAssetManagerUtil::CancelHandle(_TradeHandle);

	Super::CancelInitialization();
}

void UTradeDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeAssetByEntry);
	PrimaryCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);

	Super::NativeConstruct();
}

void UTradeDashboardUI::NativeDestruct()
{
	PrimaryCollection->OnSelectionChanged.Unbind();
	PrimaryCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}


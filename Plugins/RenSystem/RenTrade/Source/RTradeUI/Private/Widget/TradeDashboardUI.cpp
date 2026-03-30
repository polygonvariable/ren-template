// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/TradeDashboardUI.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail.h"
#include "Definition/AssetFilterProperty.h"
#include "Filter/Criterion/FilterCriterion_Leaf.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetCollection.h"
#include "Manager/RAssetManager.inl"
#include "Widget/AssetCollectionUI.h"
#include "Widget/TradeCollectionUI.h"
#include "Widget/TradeDetailUI.h"
#include "Widget/TradeEntry.h"



void UTradeDashboardUI::ResetDetail()
{
	PrimaryDetail->ResetDetail();
	SecondaryCollection->ClearEntries(true);
}


const UAssetCollection* UTradeDashboardUI::GetTradeMaterialCollection(const UCoreDataAsset* Asset) const
{
	return nullptr;
}

void UTradeDashboardUI::InitializeTradeDetails(const UTradeAsset* Asset)
{
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


void UTradeDashboardUI::InitializeDetail()
{
	if (!IsValid(AssetManager) || !TradeAssetId.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Entry, AssetManager or AssetId is invalid"));
		return;
	}

	AssetManager->CancelFetch(_TradeLoadId);

	_TradeLoadId = FGuid::NewGuid();

	TFuture<FLatentLoadedAsset<UTradeAsset>> Future = AssetManager->FetchPrimaryAsset<UTradeAsset>(_TradeLoadId, TradeAssetId);
	if (!Future.IsValid())
	{
		LOG_ERROR(LogAsset, TEXT("Failed to create Future"));
		return;
	}

	TWeakObjectPtr<UTradeDashboardUI> WeakThis(this);
	Future.Next([WeakThis](const FLatentLoadedAsset<UTradeAsset>& Result)
		{
			UTradeDashboardUI* This = WeakThis.Get();
			if (IsValid(This) && Result.IsValid())
			{
				This->InitializeTradeDetails(Result.Get());
			}
		}
	);
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

void UTradeDashboardUI::NativeConstruct()
{
	PrimaryCollection->OnSelectionChanged.BindUObject(this, &UAssetDashboardUI::InitializeAssetByEntry);
	PrimaryCollection->OnSelectionCleared.BindUObject(this, &UAssetDashboardUI::ResetDetail);

	Super::NativeConstruct();
}

void UTradeDashboardUI::NativeDestruct()
{
	if (IsValid(AssetManager))
	{
		AssetManager->CancelFetch(_TradeLoadId);
	}

	PrimaryCollection->OnSelectionChanged.Unbind();
	PrimaryCollection->OnSelectionCleared.Unbind();

	Super::NativeDestruct();
}


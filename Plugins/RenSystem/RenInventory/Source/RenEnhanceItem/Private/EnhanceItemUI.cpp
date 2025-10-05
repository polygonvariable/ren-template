// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EnhanceItemUI.h"

// Engine Headers
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceProviderInterface.h"
#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreFilter/Public/FilterGroup.h"
#include "RCoreFilter/Public/FilterLeafCriterion.h"

#include "RCoreInventory/Public/InventoryRecord.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryDefinition.h"
#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenInventory/Public/Widget/InventoryCollectionUI.h"
#include "RenInventory/Public/Widget/InventoryDetailUI.h"

#include "RenEnhanceItem/Public/EnhanceItemSubsystem.h"



void UEnhanceItemUI::InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (!Record || !AssetId.IsValid())
	{
		return;
	}

	ActiveAssetId = AssetId;
	ActiveItemId = Record->ItemId;
	ActiveEnhancement = Record->Enhancement;

	TWeakObjectPtr<UEnhanceItemUI> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, AssetId](bool, UObject* LoadedAsset)
		{
			UEnhanceItemUI* Outer = WeakThis.Get();
			if (IsValid(Outer))
			{
				Outer->HandleAssetLoaded(AssetId, LoadedAsset);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));

	if (ItemDetail)
	{
		ItemDetail->InitializeDetails(AssetId, Quantity, Record);
	}
}

void UEnhanceItemUI::ResetDetails()
{
	ActiveItemId = NAME_None;
	ActiveAsset = nullptr;
}

void UEnhanceItemUI::RefreshDetails()
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (!IsValid(Inventory))
	{
		return;
	}

	const FInventoryRecord* Record = Inventory->GetRecordById(ContainerId, ActiveAssetId, ActiveItemId);
	if (!Record)
	{
		return;
	}

	ActiveEnhancement = Record->Enhancement;

	DisplayRankUpItems();
}


void UEnhanceItemUI::TryLevelUp(const FPrimaryAssetId& AssetId, FName ItemId)
{
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	if (IsValid(EnhanceItem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		EnhanceItem->LevelUpItem(ContainerId, ActiveAssetId, ActiveItemId, AssetId, ItemId);
	}
}

void UEnhanceItemUI::TryRankUp()
{
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	if (IsValid(EnhanceItem) && ActiveAssetId.IsValid() && ActiveItemId.IsValid())
	{
		EnhanceItem->RankUpItem(ContainerId, ActiveAssetId, ActiveItemId);
	}
}


void UEnhanceItemUI::DisplayLevelUpItems()
{
	IEnhanceProviderInterface* EnhanceProvider = ActiveAsset.Get();
	if (!LevelUpList || !EnhanceProvider || !ActiveItemId.IsValid())
	{
		return;
	}

	const FExchangeRule& EnhanceRules = EnhanceProvider->GetEnhanceRules();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = EnhanceRules.RequiredAssets;
	const TMap<FName, int>& RequiredArbitrary = EnhanceRules.RequiredArbitrary;

	UFilterAssetCriterion* AssetCriterion = LevelUpList->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
	if (IsValid(AssetCriterion))
	{
		TArray<FPrimaryAssetId> AssetIds;
		RequiredAssets.GetKeys(AssetIds);

		AssetCriterion->Included.Empty();
		AssetCriterion->Included.Append(AssetIds);
	}

	UFilterTextCriterion* AssetTypeCriterion = LevelUpList->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::AssetType);
	if (IsValid(AssetTypeCriterion))
	{
		TArray<FName> AssetTypes;
		RequiredArbitrary.GetKeys(AssetTypes);

		AssetTypeCriterion->Included.Empty();
		AssetTypeCriterion->Included.Append(AssetTypes);
	}

	UFilterTextCriterion* ItemIdCriterion = LevelUpList->GetCriterionByName<UFilterTextCriterion>(InventoryFilterProperty::ItemId);
	if (IsValid(ItemIdCriterion))
	{
		ItemIdCriterion->Included.Empty();
		ItemIdCriterion->Included.Add(ActiveItemId);
	}

	LevelUpList->RefreshItems();
}

void UEnhanceItemUI::DisplayRankUpItems()
{
	UEnhanceItemSubsystem* EnhanceItem = EnhanceItemSubsystem.Get();
	IEnhanceProviderInterface* EnhanceProvider = ActiveAsset.Get();

	if (!EnhanceItem || !EnhanceProvider)
	{
		return;
	}

	bool bCanRankUp = EnhanceItem->CanRankUp(ActiveEnhancement, EnhanceProvider);

	UInventoryUI::LockControls({ RankUpButton }, bCanRankUp);

	if (RankUpList)
	{
		int CurrentRank = ActiveEnhancement.Rank;

		const TArray<FExchangeRule>& RankingRules = ActiveAsset->GetRankingRules();
		bool bIsRankValid = bCanRankUp && RankingRules.IsValidIndex(CurrentRank);

		if (bIsRankValid)
		{
			RankUpList->ClearPayloads();

			const TMap<FPrimaryAssetId, int>& RequiredAssets = RankingRules[CurrentRank].RequiredAssets;
			for (const TPair<FPrimaryAssetId, int>& Kv : RequiredAssets)
			{
				const FPrimaryAssetId& AssetId = Kv.Key;
				int Quantity = Kv.Value;

				FInventoryPayloadQuantity Payload;
				Payload.Quantity = Quantity;

				RankUpList->AddPayload(AssetId, FInstancedStruct::Make(Payload));
			}

			UFilterAssetCriterion* AssetCriterion = RankUpList->GetCriterionByName<UFilterAssetCriterion>(InventoryFilterProperty::AssetId);
			if (IsValid(AssetCriterion))
			{
				TArray<FPrimaryAssetId> AssetIds;
				RequiredAssets.GetKeys(AssetIds);

				AssetCriterion->Included.Empty();
				AssetCriterion->Included.Append(AssetIds);
			}

			RankUpList->RefreshItems();
		}
	}
}


void UEnhanceItemUI::SwitchDetails(bool bPrimary)
{
	if (RankUpSwitch) RankUpSwitch->SetActiveWidgetIndex(bPrimary ? 1 : 0);
}

void UEnhanceItemUI::LockControls(bool bLock)
{
	if (LevelUpList) LevelUpList->SetIsEnabled(!bLock);
	if (RankUpList) RankUpList->SetIsEnabled(!bLock);
	if (RankUpButton) RankUpButton->SetIsEnabled(!bLock);
}


void UEnhanceItemUI::HandleAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset)
{
	IEnhanceProviderInterface* EnhanceProvider = Cast<IEnhanceProviderInterface>(LoadedAsset);
	if (!EnhanceProvider)
	{
		return;
	}

	ActiveAsset = TWeakInterfacePtr<IEnhanceProviderInterface>(EnhanceProvider);

	DisplayLevelUpItems();
	DisplayRankUpItems();
}

void UEnhanceItemUI::HandleItemSelected(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record)
{
	if (Record)
	{
		TryLevelUp(AssetId, Record->ItemId);
	}
}


void UEnhanceItemUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (LevelUpList)
	{
		LevelUpList->QueryRule.ContainerId = ContainerId;
		LevelUpList->QueryRule.QuerySource = EInventoryQuerySource::Inventory;
	}

	if (RankUpList)
	{
		RankUpList->QueryRule.ContainerId = ContainerId;
		LevelUpList->QueryRule.QuerySource = EInventoryQuerySource::Glossary;
	}

	if (ItemDetail)
	{
		ItemDetail->ContainerId = ContainerId;
	}
}

void UEnhanceItemUI::NativeConstruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UInventorySubsystem* Inventory = GameInstance->GetSubsystem<UInventorySubsystem>();
		if (IsValid(Inventory))
		{
			if (bAutoRefresh)
			{
				Inventory->OnItemAdded.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
				Inventory->OnItemRemoved.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
				Inventory->OnItemUpdated.AddWeakLambda(this, [this](FName ContainerId, const FPrimaryAssetId&, FName) { if (this->ContainerId == ContainerId && bAutoRefresh) RefreshDetails(); });
			}
			InventorySubsystem = Inventory;
		}
		EnhanceItemSubsystem = GameInstance->GetSubsystem<UEnhanceItemSubsystem>();
	}

	if (LevelUpList)
	{
		LevelUpList->OnItemSelected.RemoveAll(this);
		LevelUpList->OnItemSelected.AddUObject(this, &UEnhanceItemUI::HandleItemSelected);
	}
	if (RankUpButton)
	{
		RankUpButton->OnClicked.RemoveAll(this);
		RankUpButton->OnClicked.AddDynamic(this, &UEnhanceItemUI::TryRankUp);
	}

	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveAll(this);
		CloseButton->OnClicked.AddDynamic(this, &UEnhanceItemUI::CloseWidget);
	}

	Super::NativeConstruct();
}

void UEnhanceItemUI::NativeDestruct()
{
	ResetDetails();

	if (LevelUpList) LevelUpList->OnItemSelected.RemoveAll(this);
	if (RankUpButton) RankUpButton->OnClicked.RemoveAll(this);
	if (CloseButton) CloseButton->OnClicked.RemoveAll(this);

	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	if (IsValid(Inventory))
	{
		Inventory->OnItemAdded.RemoveAll(this);
		Inventory->OnItemRemoved.RemoveAll(this);
		Inventory->OnItemUpdated.RemoveAll(this);
	}
	InventorySubsystem.Reset();

	EnhanceItemSubsystem.Reset();

	Super::NativeDestruct();
}


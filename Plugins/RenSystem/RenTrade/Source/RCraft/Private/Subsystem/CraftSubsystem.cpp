// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/CraftSubsystem.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail_Trade.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/TradeKey.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/ICraftProvider.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Settings/CraftSettings.h"
#include "Storage/CraftStorage.h"
#include "Storage/CraftStorageManager.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "Task/Task_ClaimCraftItem.h"
#include "Task/Task_CraftItem.h"


UCraftStorageManager* UCraftSubsystem::GetStorageManager()
{
	if (!StorageProvider)
	{
		return nullptr;
	}
	FName StorageId = UCraftSettings::Get()->StorageId;
	return StorageProvider->GetStorageManager<UCraftStorageManager>(StorageId);
}


bool UCraftSubsystem::TryClaimCraftItem(const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UTask_ClaimCraftItem* Action = AuthActionSubsystem->CreateAction<UTask_ClaimCraftItem>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		return false;
	}

	Action->CraftAssetId = CraftAssetId;
	Action->TargetAssetId = TargetAssetId;
	Action->TradeCollectionId = TradeCollectionId;
	return Action->StartAction();
}

bool UCraftSubsystem::TryCraftItem(const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UTask_CraftItem* Action = AuthActionSubsystem->CreateAction<UTask_CraftItem>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		return false;
	}

	Action->CraftAssetId = CraftAssetId;
	Action->TargetAssetId = TargetAssetId;
	Action->TradeCollectionId = TradeCollectionId;
	return Action->StartAction();
}


const UAssetCollection* UCraftSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const
{
	const ICraftProvider* Provider = Cast<ICraftProvider>(Asset);
	if (!Provider)
	{
		return nullptr;
	}
	return Provider->GetCraftingMaterial(Context);
}

const UAssetCollection* UCraftSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}


void UCraftSubsystem::QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback)
{
	UCraftStorageManager* StorageManager = GetStorageManager();
	if (!IsValid(Asset) || !IsValid(StorageManager))
	{
		return;
	}

	const UAssetGroup* TradeGroup = Asset->TradeGroup;
	if (!IsValid(TradeGroup))
	{
		return;
	}

	FInstancedStruct Context = FInstancedStruct::Make(FAssetRuleContext(CollectionId));
	const UAssetCollection_Trade* AssetCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(Context);
	if (!IsValid(AssetCollection))
	{
		return;
	}

	FPrimaryAssetId CraftAssetId = Asset->GetPrimaryAssetId();
	const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList = AssetCollection->GetAssetList();

	if (QuerySource == ECraftQuerySource::Glossary)
	{
		QueryAssetItems(AssetList, CraftAssetId, CollectionId, Context, StorageManager, MoveTemp(Callback));
	}
	else
	{
		QueryStorageItems(AssetList, CraftAssetId, CollectionId, Context, StorageManager, MoveTemp(Callback));
	}
}

void UCraftSubsystem::QueryAssetItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& AssetKv : AssetList)
	{
		const UCoreDataAsset* ItemDataAsset = AssetKv.Key;
		FAssetDetail_Trade ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftData* CraftData = StorageManager->GetItem(TradeKey);
		if (CraftData)
		{
			ItemDetail.Quota = FMath::Max(0, ItemDetail.Quota - CraftData->PendingQuantity);
		}

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}

void UCraftSubsystem::QueryStorageItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
{
	for (const TPair<UCoreDataAsset*, FAssetDetail_Trade>& AssetKv : AssetList)
	{
		const UCoreDataAsset* ItemDataAsset = AssetKv.Key;
		FAssetDetail_Trade ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftData* CraftData = StorageManager->GetItem(TradeKey);
		if (!CraftData)
		{
			continue;
		}

		ItemDetail.Quota = CraftData->PendingQuantity;

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}


void UCraftSubsystem::OnPreGameInitialized()
{
	StorageProvider = IStorageProvider::Get(GetGameInstance());
	if (!StorageProvider)
	{
		LOG_ERROR(LogShop, TEXT("Storage subsystem not found"));
		return;
	}

	const UCraftSettings* Settings = UCraftSettings::Get();

	FStorageDefinition Definition;
	Definition.StorageId = Settings->StorageId;
	Definition.StorageClass = Settings->StorageClass;
	Definition.ManagerClass = Settings->StorageManagerClass;

	StorageProvider->LoadStorage(Definition, FTaskCallback());
}

bool UCraftSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UCraftSettings::Get()->SubsystemClass;
}

void UCraftSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("CraftSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UCraftSubsystem::OnPreGameInitialized);
}

void UCraftSubsystem::Deinitialize()
{
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);
	StorageProvider = nullptr;

	LOG_WARNING(LogTemp, TEXT("CraftSubsystem deinitialized"));
	Super::Deinitialize();
}


UCraftSubsystem* UCraftSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UCraftSubsystem* UCraftSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UCraftSubsystem>();
}


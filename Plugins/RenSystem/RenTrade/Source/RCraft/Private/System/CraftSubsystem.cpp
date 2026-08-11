// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/CraftSubsystem.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Auth/AACraftClaimItem.h"
#include "Auth/AACraftItem.h"
#include "Core/CraftSettings.h"
#include "Core/Type/Runtime/TradeKey.h"
#include "Core/Type/TradeAssetDetail.h"
#include "Data/CraftFragment.h"
#include "Data/CraftStorage.h"
#include "Data/TradeAsset.h"
#include "Data/TradeAssetCollection.h"
#include "Definition/AssetRuleDefinition.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "System/CraftStorageManager.h"


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
	UAACraftClaimItem* Action = AuthActionSubsystem->CreateAction<UAACraftClaimItem>(ActionId);
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
	UAACraftItem* Action = AuthActionSubsystem->CreateAction<UAACraftItem>(ActionId);
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
	const UCraftFragment* CraftFragment = Asset->FindFragmentByClass<UCraftFragment>();
	if (!IsValid(CraftFragment))
	{
		return nullptr;
	}
	return CraftFragment->GetCraftingMaterial(Context);
}

const UAssetCollection* UCraftSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}


void UCraftSubsystem::QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)> Callback)
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
	const UTradeAssetCollection* AssetCollection = TradeGroup->GetCollectionRule<UTradeAssetCollection>(Context);
	if (!IsValid(AssetCollection))
	{
		return;
	}

	FPrimaryAssetId CraftAssetId = Asset->GetPrimaryAssetId();
	const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList = AssetCollection->GetAssetList();

	if (QuerySource == ECraftQuerySource::Glossary)
	{
		QueryAssetItems(AssetList, CraftAssetId, CollectionId, Context, StorageManager, MoveTemp(Callback));
	}
	else
	{
		QueryStorageItems(AssetList, CraftAssetId, CollectionId, Context, StorageManager, MoveTemp(Callback));
	}
}

void UCraftSubsystem::QueryAssetItems(const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)>&& Callback)
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& AssetKv : AssetList)
	{
		const UCoreDataAsset* ItemDataAsset = AssetKv.Key;
		FTradeAssetDetail ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftInstance* CraftData = StorageManager->GetItem(TradeKey);
		if (CraftData)
		{
			ItemDetail.Quota = FMath::Max(0, ItemDetail.Quota - CraftData->PendingQuantity);
		}

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}

void UCraftSubsystem::QueryStorageItems(const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)>&& Callback)
{
	for (const TPair<UCoreDataAsset*, FTradeAssetDetail>& AssetKv : AssetList)
	{
		const UCoreDataAsset* ItemDataAsset = AssetKv.Key;
		FTradeAssetDetail ItemDetail = AssetKv.Value;

		const UAssetCollection* MaterialCollection = GetMaterialCollection(ItemDataAsset, Context);
		if (!IsValid(MaterialCollection))
		{
			continue;
		}

		const FPrimaryAssetId& ItemAssetId = ItemDataAsset->GetPrimaryAssetId();

		FTradeKey TradeKey(CraftAssetId, CollectionId, ItemAssetId);
		const FCraftInstance* CraftData = StorageManager->GetItem(TradeKey);
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


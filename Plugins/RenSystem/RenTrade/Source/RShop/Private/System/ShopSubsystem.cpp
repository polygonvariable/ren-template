// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/ShopSubsystem.h"

// Engine Headers
#include "StructUtils/InstancedStruct.h"

// Project Headers
#include "Auth/AAPurchaseItem.h"
#include "Core/ShopSettings.h"
#include "Core/Type/TradeAssetDetail.h"
#include "Data/ShopFragment.h"
#include "Data/ShopStorage.h"
#include "Data/TradeAsset.h"
#include "Data/TradeAssetCollection.h"
#include "Core/Type/AssetRuleDefinition.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/StorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Data/AssetGroup.h"
#include "Subsystem/AuthActionSubsystem.h"
#include "System/ShopStorageManager.h"


UShopStorageManager* UShopSubsystem::GetStorageManager()
{
	if (!StorageProvider)
	{
		return nullptr;
	}
	const UShopSettings* ShopSettings = UShopSettings::Get();
	return StorageProvider->GetStorageManager<UShopStorageManager>(ShopSettings->StorageId);
}

bool UShopSubsystem::TryPurchaseItem(const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId)
{
	UAuthActionSubsystem* AuthActionSubsystem = UAuthActionSubsystem::Get(GetGameInstance());
	if (!IsValid(AuthActionSubsystem))
	{
		LOG_ERROR(LogShop, TEXT("Task subsystem is invalid"));
		return false;
	}

	FGuid ActionId = FGuid::NewGuid();
	UAAPurchaseItem* Action = AuthActionSubsystem->CreateAction<UAAPurchaseItem>(ActionId);
	if (!IsValid(Action))
	{
		LOG_ERROR(LogShop, TEXT("Failed to create task"));
		return false;
	}

	Action->ShopAssetId = ShopAssetId;
	Action->TradeCollectionId = TradeCollectionId;
	Action->TargetAssetId = TargetAssetId;
	return Action->StartAction();
}


const UAssetCollection* UShopSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const
{
	const UShopFragment* ShopFragment = Asset->FindFragmentByClass<UShopFragment>();
	if (!IsValid(ShopFragment))
	{
		return nullptr;
	}
	return ShopFragment->GetPurchaseCost(Context);
}

const UAssetCollection* UShopSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}


void UShopSubsystem::QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&)> Callback)
{
	if (!IsValid(Asset))
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

	FPrimaryAssetId ShopAssetId = Asset->GetPrimaryAssetId();
	const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList = AssetCollection->GetAssetList();

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

		Callback(ItemAssetId, ItemDetail);
	}
}


void UShopSubsystem::OnPreGameInitialized()
{
	IStorageProvider* StorageInterface = IStorageProvider::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogShop, TEXT("Storage subsystem not found"));
		return;
	}

	const UShopSettings* Settings = UShopSettings::Get();

	FStorageDefinition Definition;
	Definition.StorageId = Settings->StorageId;
	Definition.StorageClass = Settings->StorageClass;
	Definition.ManagerClass = Settings->StorageManagerClass;

	StorageInterface->LoadStorage(Definition, FTaskCallback());
}

bool UShopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UShopSettings::Get()->SubsystemClass;
}

void UShopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogShop, TEXT("ShopSubsystem initialized"));

	FGameLifecycleDelegate::OnPreGameInitialized.AddUObject(this, &UShopSubsystem::OnPreGameInitialized);
}

void UShopSubsystem::Deinitialize()
{
	StorageProvider = nullptr;
	FGameLifecycleDelegate::OnPreGameInitialized.RemoveAll(this);

	LOG_WARNING(LogShop, TEXT("ShopSubsystem deinitialized"));
	Super::Deinitialize();
}


UShopSubsystem* UShopSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return Get(World->GetGameInstance());
}

UShopSubsystem* UShopSubsystem::Get(UGameInstance* GameInstance)
{
	if (!IsValid(GameInstance))
	{
		return nullptr;
	}
	return GameInstance->GetSubsystem<UShopSubsystem>();
}


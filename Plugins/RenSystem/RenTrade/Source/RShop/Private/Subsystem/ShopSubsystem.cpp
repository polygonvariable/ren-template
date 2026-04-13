// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/ShopSubsystem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail_Trade.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/TradeKey.h"
#include "Delegate/GameLifecycleDelegate.h"
#include "Interface/IShopProvider.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Settings/ShopSettings.h"
#include "Storage/ShopStorage.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/Task_PurchaseItem.h"



UShopStorage* UShopSubsystem::GetShop(const FName& ShopId)
{
	IStorageProvider* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UShopStorage>(ShopId);
}

void UShopSubsystem::PurchaseItem(const FGuid& TaskId, const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogShop, TEXT("Task subsystem is invalid"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	UTask_PurchaseItem* Task = TaskSubsystem->CreateTask<UTask_PurchaseItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogShop, TEXT("Failed to create task"));
		Callback.ExecuteIfBound(FTaskResult(ETaskState::Failed));
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->ShopAssetId = ShopAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->TargetAssetId = TargetAssetId;
	Task->StartTask();
}


const UAssetCollection* UShopSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const
{
	const IShopProvider* ShopProvider = Cast<IShopProvider>(Asset);
	if (!ShopProvider)
	{
		return nullptr;
	}
	return ShopProvider->GetPurchaseCost(Context);
}

const UAssetCollection* UShopSubsystem::GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const
{
	return GetMaterialCollection(Asset, FInstancedStruct::Make(FAssetRuleContext(CollectionId)));
}


void UShopSubsystem::QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&)> Callback)
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
	const UAssetCollection_Trade* AssetCollection = TradeGroup->GetCollectionRule<UAssetCollection_Trade>(Context);
	if (!IsValid(AssetCollection))
	{
		return;
	}

	FPrimaryAssetId ShopAssetId = Asset->GetPrimaryAssetId();
	const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList = AssetCollection->GetAssetList();

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

	FStorageHandle Handle;
	Handle.StorageClass = Settings->StorageClass;
	Handle.StorageId = Settings->StorageId;
	Handle.Url = Settings->StorageUrl;

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
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
	StorageProvider.Reset();

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


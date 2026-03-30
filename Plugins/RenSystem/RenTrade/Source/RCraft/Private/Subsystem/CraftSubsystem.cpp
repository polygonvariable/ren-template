// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Subsystem/CraftSubsystem.h"

// Engine Headers
#include "InstancedStruct.h"

// Project Headers
#include "Asset/TradeAsset.h"
#include "Definition/AssetDetail_Trade.h"
#include "Definition/AssetRuleDefinition.h"
#include "Definition/Runtime/TradeKey.h"
#include "Delegate/GameLifecycleDelegates.h"
#include "Interface/ICraftProvider.h"
#include "Interface/IStorageProvider.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Management/AssetGroup.h"
#include "Management/Collection/AssetCollection_Trade.h"
#include "Settings/CraftSettings.h"
#include "Storage/CraftStorage.h"
#include "Subsystem/TaskSubsystem.h"
#include "Task/Task_ClaimCraftItem.h"
#include "Task/Task_CraftItem.h"



UCraftStorage* UCraftSubsystem::GetCraft(const FName& CraftId)
{
	IStorageProvider* StorageInterface = StorageProvider.Get();
	if (!StorageInterface)
	{
		return nullptr;
	}
	return StorageInterface->GetStorage<UCraftStorage>(CraftId);
}



void UCraftSubsystem::ClaimCraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		Callback.ExecuteIfBound(ETaskState::Failed);
		return;
	}

	UTask_ClaimCraftItem* Task = TaskSubsystem->CreateTask<UTask_ClaimCraftItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		Callback.ExecuteIfBound(ETaskState::Failed);
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->CraftAssetId = CraftAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->StartTask();
}

void UCraftSubsystem::CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback)
{
	UTaskSubsystem* TaskSubsystem = UTaskSubsystem::Get(GetGameInstance());
	if (!IsValid(TaskSubsystem))
	{
		LOG_ERROR(LogCraft, TEXT("Task subsystem is invalid"));
		Callback.ExecuteIfBound(ETaskState::Failed);
		return;
	}

	UTask_CraftItem* Task = TaskSubsystem->CreateTask<UTask_CraftItem>(TaskId);
	if (!IsValid(Task))
	{
		LOG_ERROR(LogCraft, TEXT("Failed to create task"));
		Callback.ExecuteIfBound(ETaskState::Failed);
		return;
	}

	Task->Callback = MoveTemp(Callback);
	Task->CraftAssetId = CraftAssetId;
	Task->TargetAssetId = TargetAssetId;
	Task->TradeCollectionId = TradeCollectionId;
	Task->StartTask();
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



void UCraftSubsystem::QueryItems(const FName& CraftId, const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback)
{
	UCraftStorage* CraftStorage = GetCraft(CraftId);
	if (!IsValid(Asset) || !IsValid(CraftStorage))
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
		QueryAssetItems(AssetList, CraftAssetId, CollectionId, Context, CraftStorage, MoveTemp(Callback));
	}
	else
	{
		HandleStorageItems(AssetList, CraftAssetId, CollectionId, Context, CraftStorage, MoveTemp(Callback));
	}
}

void UCraftSubsystem::QueryAssetItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
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
		const FCraftData* CraftData = CraftStorage->GetItem(TradeKey);
		if (CraftData)
		{
			ItemDetail.Quota = FMath::Max(0, ItemDetail.Quota - CraftData->PendingQuantity);
		}

		Callback(ItemAssetId, ItemDetail, CraftData);
	}
}

void UCraftSubsystem::HandleStorageItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback)
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
		const FCraftData* CraftData = CraftStorage->GetItem(TradeKey);
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
	IStorageProvider* StorageInterface = IStorageProvider::Get(GetGameInstance());
	if (!StorageInterface)
	{
		LOG_ERROR(LogShop, TEXT("Storage subsystem not found"));
		return;
	}

	const UCraftSettings* Settings = UCraftSettings::Get();

	FStorageHandle Handle;
	Handle.StorageClass = Settings->StorageClass;
	Handle.StorageId = Settings->StorageId;
	Handle.Url = Settings->StorageUrl;

	StorageInterface->LoadStorage(MoveTemp(Handle));

	StorageProvider = TWeakInterfacePtr<IStorageProvider>(StorageInterface);
}

bool UCraftSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetClass() == UCraftSettings::Get()->SubsystemClass;
}

void UCraftSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("CraftSubsystem initialized"));

	FGameLifecycleDelegates::OnPreGameInitialized.AddUObject(this, &UCraftSubsystem::OnPreGameInitialized);
}

void UCraftSubsystem::Deinitialize()
{
	StorageProvider.Reset();
	FGameLifecycleDelegates::OnPreGameInitialized.RemoveAll(this);

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


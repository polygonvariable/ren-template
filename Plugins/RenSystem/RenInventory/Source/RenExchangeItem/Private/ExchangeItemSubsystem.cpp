// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "ExchangeItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreCounter/Public/CounterContainer.h"
#include "RCoreCounter/Public/CounterManagerInterface.h"

#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreExchange/Public/ExchangeQuota.h"
#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"
#include "RCoreLibrary/Public/SubsystemUtils.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"



void UExchangeItemSubsystem::Exchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("AssetId, AssetManager is invalid or Quantity is <= 0"));
		OnItemExchanged.Broadcast(false);
		return;
	}

	TWeakObjectPtr<UExchangeItemSubsystem> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, ContainerId, AssetId, Quantity](bool, UObject* LoadedObject)
		{
			UExchangeItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleItemExchange(ContainerId, AssetId, Quantity, LoadedObject);
				Owner->OnItemExchanged.Broadcast(bResult);
			}
		};
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

void UExchangeItemSubsystem::Exchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FName CounterId, const FExchangeQuota& Quota)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("AssetId, AssetManager is invalid or Quantity is <= 0"));
		OnItemExchanged.Broadcast(false);
		return;
	}

	TWeakObjectPtr<UExchangeItemSubsystem> WeakThis(this);
	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, ContainerId, AssetId, Quantity, CounterId, Quota](bool, UObject* LoadedObject)
		{
			UExchangeItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleItemExchange(ContainerId, AssetId, Quantity, LoadedObject, CounterId, Quota);
				Owner->OnItemExchanged.Broadcast(bResult);
			}
		};
	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}


const FExchangeRule* UExchangeItemSubsystem::GetExchangeRule(UObject* Target) const
{
	return nullptr;
}

bool UExchangeItemSubsystem::HandleItemExchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	const FExchangeRule* ExchangeRule = GetExchangeRule(AssetObject);
	if (!IsValid(Inventory) || !ExchangeRule)
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("InventorySubsystem, ExchangeRule is invalid"));
		return false;
	}

	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule->RequiredAssets;

	bool bContains = Inventory->ContainsItems(ContainerId, RequiredAssets, Quantity);
	if (!bContains)
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Inventory does not contain required items"));
		return false;
	}

	bool bRemoved = Inventory->RemoveItems(ContainerId, RequiredAssets, Quantity);
	if (!bRemoved)
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Failed to consume required items"));
		return false;
	}

	bool bAdded = Inventory->AddItem(ContainerId, AssetId, Quantity);
	if (!bAdded)
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Failed to add item"));
		return false;
	}

	return true;
}

bool UExchangeItemSubsystem::HandleItemExchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject, FName CounterId, const FExchangeQuota& Quota)
{
	ICounterManagerInterface* CounterManager = CounterManagerInterface.Get();
	if (!CounterManager)
	{
		PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("CounterManager is invalid"));
		return false;
	}

	if (!Quota.bUnlimited)
	{
		const FCounterRecord* Record = CounterManager->FindOrAddCounter(CounterId, AssetId.PrimaryAssetName);
		if (!Record)
		{
			PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Failed to find or add counter"));
			return false;
		}

		bool bExhausted = Record->Value >= Quota.Limit;
		if (bExhausted)
		{
			PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Exchange limit reached"));
			return false;
		}

		bool bCooldown = Quota.Cooldown.IsZero();
		if (!bCooldown)
		{
			FTimespan CooldownTime = Quota.Cooldown;
			FDateTime CurrentTime = FDateTime::UtcNow();

			if ((Record->Timestamp + CooldownTime) > CurrentTime)
			{
				PRINT_ERROR(LogExchangeItem, 1.0f, TEXT("Item is in cooldown"));
				return false;
			}
		}
	}

	bool bExchanged = HandleItemExchange(ContainerId, AssetId, Quantity, AssetObject);
	if (!bExchanged)
	{
		return false;
	}

	if (!Quota.bUnlimited)
	{
		int NewValue = CounterManager->IncrementCounter(CounterId, AssetId.PrimaryAssetName, Quota.Limit);
		return NewValue > 0;
	}

	return true;
}


void UExchangeItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UGameInstance* GameInstance = GetGameInstance();
	InventorySubsystem = GameInstance->GetSubsystem<UInventorySubsystem>();

	ICounterManagerInterface* CounterManager = SubsystemUtils::GetSubsystemInterface<UGameInstance, UGameInstanceSubsystem, ICounterManagerInterface>(GameInstance);
	CounterManagerInterface = TWeakInterfacePtr<ICounterManagerInterface>(CounterManager);
	
	AssetManager = UAssetManager::GetIfInitialized();
}

bool UExchangeItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return false;
}

void UExchangeItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogExchangeItem, TEXT("Initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UExchangeItemSubsystem::HandleGameInitialized);
}

void UExchangeItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	CounterManagerInterface.Reset();
	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogExchangeItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}


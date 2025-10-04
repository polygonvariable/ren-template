// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "CraftItemSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"

#include "RCoreExchange/Public/ExchangeRule.h"

#include "RCoreCraft/Public/CraftProviderInterface.h"

#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventoryPrimaryAsset.h"
#include "RenInventory/Public/InventorySubsystem.h"

#include "RenCounter/Public/CounterSubsystem.h"
#include "RCoreCounter/Public/CounterContainer.h"





void UCraftItemSubsystem::CraftItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("AssetId, AssetManager is invalid or Quantity is <= 0"));
		OnItemCrafted.Broadcast(false);
		return;
	}

	TWeakObjectPtr<UCraftItemSubsystem> WeakThis(this);

	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, ContainerId, AssetId, Quantity](bool, UObject* LoadedObject)
		{
			UCraftItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleItemCrafting(ContainerId, AssetId, Quantity, LoadedObject);
				Owner->OnItemCrafted.Broadcast(bResult);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}

void UCraftItemSubsystem::CraftItem(FName CraftId, FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, const FExchangeQuota& Quota)
{
	if (!InventoryPrimaryAsset::IsValid(AssetId) || Quantity <= 0 || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("AssetId, AssetManager is invalid or Quantity is <= 0"));
		OnItemCrafted.Broadcast(false);
		return;
	}

	TWeakObjectPtr<UCraftItemSubsystem> WeakThis(this);

	TFunction<void(bool, UObject*)> AsyncCallback = [WeakThis, CraftId, ContainerId, AssetId, Quantity, Quota](bool, UObject* LoadedObject)
		{
			UCraftItemSubsystem* Owner = WeakThis.Get();
			if (IsValid(Owner))
			{
				bool bResult = Owner->HandleItemCrafting(CraftId, ContainerId, AssetId, Quantity, Quota, LoadedObject);
				Owner->OnItemCrafted.Broadcast(bResult);
			}
		};

	AssetManagerUtils::LoadPrimaryAsset(this, AssetManager, AssetId, MoveTemp(AsyncCallback));
}


bool UCraftItemSubsystem::ResetCrafter(FName CraftId)
{
	UCounterSubsystem* Counter = GetGameInstance()->GetSubsystem<UCounterSubsystem>();
	if (!IsValid(Counter))
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("CounterProvider is invalid"));
		return false;
	}

	bool bResult = Counter->ResetCounter(CraftId);
	return bResult;
}


bool UCraftItemSubsystem::HandleItemCrafting(FName CraftId, FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, const FExchangeQuota& Quota, UObject* AssetObject)
{
	UCounterSubsystem* Counter = CounterSubsystem.Get();
	if (!IsValid(Counter))
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("CounterProvider is invalid"));
		return false;
	}

	if (!Quota.bUnlimited)
	{
		const FCounterRecord* Record = Counter->FindOrAddCounter(CraftId, AssetId.PrimaryAssetName);
		if (!Record)
		{
			PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Failed to find or add counter"));
			return false;
		}

		if (Record->Value >= Quota.Limit)
		{
			PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Craft limit reached"));
			return false;
		}

		bool bCooldown = Quota.Cooldown.IsZero();
		if (!bCooldown)
		{
			FTimespan CooldownTime = Quota.Cooldown;
			FDateTime CurrentTime = FDateTime::UtcNow();

			if ((Record->Timestamp + CooldownTime) > CurrentTime)
			{
				PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Item is in cooldown"));
				return false;
			}
		}
	}

	bool bCrafted = HandleItemCrafting(ContainerId, AssetId, Quantity, AssetObject);
	if (!bCrafted)
	{
		return false;
	}

	if (!Quota.bUnlimited)
	{
		int NewValue = Counter->IncrementCounter(CraftId, AssetId.PrimaryAssetName, Quota.Limit);
		return NewValue > 0;
	}

	return true;
}

bool UCraftItemSubsystem::HandleItemCrafting(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject)
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(AssetObject);
	if (!IsValid(Inventory) || !CraftProvider)
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("InventorySubsystem, CraftProvider is invalid"));
		return false;
	}

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	const TMap<FPrimaryAssetId, int>& RequiredAssets = ExchangeRule.RequiredAssets;

	bool bCanCraft = Inventory->ContainsItems(ContainerId, RequiredAssets, Quantity);
	if (!bCanCraft)
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Inventory does not contain required items"));
		return false;
	}

	bool bRemoved = Inventory->RemoveItems(ContainerId, RequiredAssets, Quantity);
	if (!bRemoved)
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Failed to consume required items"));
		return false;
	}

	bool bAdded = Inventory->AddItem(ContainerId, AssetId, Quantity);
	if (!bAdded)
	{
		PRINT_ERROR(LogCraftItem, 1.0f, TEXT("Failed to add item"));
		return false;
	}

	return true;
}



void UCraftItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	CounterSubsystem = GetGameInstance()->GetSubsystem<UCounterSubsystem>();
	InventorySubsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	AssetManager = UAssetManager::GetIfInitialized();
}

bool UCraftItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UCraftItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCraftItem, TEXT("Initialized"));

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UCraftItemSubsystem::HandleGameInitialized);
	}
}

void UCraftItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogCraftItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}


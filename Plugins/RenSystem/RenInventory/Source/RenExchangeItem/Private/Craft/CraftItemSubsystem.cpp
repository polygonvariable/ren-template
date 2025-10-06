// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Craft/CraftItemSubsystem.h"

// Engine Headers

// Project Headers
#include "RCoreCraft/Public/CraftProviderInterface.h"
#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"

#include "RenInventory/Public/InventorySubsystem.h"



const FExchangeRule* UCraftItemSubsystem::GetExchangeRule(UObject* Target) const
{
	UInventorySubsystem* Inventory = InventorySubsystem.Get();
	ICraftProviderInterface* CraftProvider = Cast<ICraftProviderInterface>(Target);
	if (!IsValid(Inventory) || !CraftProvider)
	{
		return nullptr;
	}

	const FExchangeRule& ExchangeRule = CraftProvider->GetCraftingRule();
	return &ExchangeRule;
}


bool UCraftItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UCraftItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogCraftItem, TEXT("Initialized"));

	FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UCraftItemSubsystem::HandleGameInitialized);
}

void UCraftItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();
	AssetManager = nullptr;

	LOG_WARNING(LogCraftItem, TEXT("Deinitialized"));
	Super::Deinitialize();
}


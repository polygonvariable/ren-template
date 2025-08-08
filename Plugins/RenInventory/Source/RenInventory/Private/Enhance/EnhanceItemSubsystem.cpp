// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Enhance/EnhanceItemSubsystem.h"

// Project Headers
#include "RenAsset/Public/AssetMetadata.h"

#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"
#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

#include "RenCore/Public/Delegate/LatentDelegates.h"
#include "RenCore/Public/Library/EnhanceLibrary.h"
#include "RenCore/Public/Macro/LogMacro.h"
#include "RenCore/Public/Record/InventoryRecord.h"

#include "RenInventory/Public/InventorySubsystem.h"



bool UEnhanceItemSubsystem::LevelUpItem(FName ContainerId, FName ItemGuid, FName EnhanceGuid)
{
	if (ItemGuid == EnhanceGuid)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item and enhance are the same"));
		return false;
	}

	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return false;
	}

	const FInventoryRecord* ItemRecord = Subsystem->GetItemRecord(ContainerId, ItemGuid);
	if (!ItemRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		return false;
	}

	UEnhanceableAsset* ItemAsset = Subsystem->GetItemAsset<UEnhanceableAsset>(ItemRecord->ItemId);
	if (!ItemAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item asset is invalid"));
		return false;
	}
	
	const FInventoryRecord* EnhanceRecord = Subsystem->GetItemRecord(ContainerId, EnhanceGuid);
	if (!EnhanceRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Enhance record is invalid"));
		return false;
	}

	UEnhanceAsset* EnhanceAsset = Subsystem->GetItemAsset<UEnhanceAsset>(EnhanceRecord->ItemId);
	if (EnhanceAsset)
	{
		if (ItemAsset->EnhanceCosts.Contains(EnhanceAsset))
		{
			return HandleLevelUp(
				Subsystem,
				ContainerId,
				ItemGuid,
				EnhanceGuid,
				ItemRecord,
				ItemAsset,
				EnhanceAsset->ExchangePoints
			);
		}
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Internal enhance not found, trying to check for external enhance"));
	}

	int* EnhancePoints = ItemAsset->ExternalCosts.Find(EnhanceAsset->ItemType);
	if (!EnhancePoints)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("External enhance failed and no points found"));
		return false;
	}

	return HandleLevelUp(
		Subsystem,
		ContainerId,
		ItemGuid,
		EnhanceGuid,
		ItemRecord,
		ItemAsset,
		*EnhancePoints
	);
}

bool UEnhanceItemSubsystem::RankUpItem(FName ContainerId, FName ItemGuid)
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return false;
	}

	const FInventoryRecord* ItemRecord = Subsystem->GetItemRecord(ContainerId, ItemGuid);
	if (!ItemRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		return false;
	}

	UEnhanceableAsset* ItemAsset = Subsystem->GetItemAsset<UEnhanceableAsset>(ItemRecord->ItemId);
	if (!ItemAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item asset is invalid"));
		return false;
	}

	int NextRank = ItemRecord->EnhanceRecord.Rank;
	const TArray<FInventoryAssetQuantity>& RankingCosts = ItemAsset->RankingCosts;
	if (!RankingCosts.IsValidIndex(NextRank))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Ranking cost is invalid"));
		return false;
	}

	const FInventoryAssetQuantity& CostItem = RankingCosts[NextRank];
	const TMap<FName, int>& ItemQuantities = CostItem.GetItemIds();
	if (!Subsystem->ContainsItems(ContainerId, ItemQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Not enough items to rank up"));
		return false;
	}
	
	return HandleRankUp(Subsystem, ContainerId, ItemGuid, ItemRecord, ItemAsset, ItemQuantities);
}


bool UEnhanceItemSubsystem::CanRankUp(FName ContainerId, FName ItemGuid) const
{
	UInventorySubsystem* Subsystem = InventorySubsystem.Get();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return false;
	}

	const FInventoryRecord* ItemRecord = Subsystem->GetItemRecord(ContainerId, ItemGuid);
	if (!ItemRecord)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item record is invalid"));
		return false;
	}

	UEnhanceableAsset* ItemAsset = Subsystem->GetItemAsset<UEnhanceableAsset>(ItemRecord->ItemId);
	if (!ItemAsset)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Item asset is invalid"));
		return false;
	}

	int CurrentXp = ItemRecord->EnhanceRecord.Experience;
	int CurrentLevel = ItemRecord->EnhanceRecord.Level;
	int CurrentRank = ItemRecord->EnhanceRecord.Rank;
	int LevelInterval = ItemAsset->GetLevelInterval(CurrentRank);

	return UEnhanceLibrary::CanRankUp(CurrentXp, CurrentLevel, LevelInterval, CurrentRank);
}


bool UEnhanceItemSubsystem::HandleLevelUp(UInventorySubsystem* Subsystem, FName ContainerId, FName ItemGuid, FName EnhanceGuid, const FInventoryRecord* ItemRecord, UEnhanceableAsset* ItemAsset, int EnhancePoint)
{
	int CurrentXp = ItemRecord->EnhanceRecord.Experience;
	int CurrentLevel = ItemRecord->EnhanceRecord.Level;
	int CurrentRank = ItemRecord->EnhanceRecord.Rank;

	int NewXp = 0;
	int NewLevel = 0;

	bool bDoesLevelUpdated = false;
	bool bDoesRankShouldUpdate = false;
	bool bDoesMaxLevelReached = false;

	bool bCanLevelUp = UEnhanceLibrary::CalculateLevelUp(
		EnhancePoint,
		CurrentXp,
		CurrentLevel,
		CurrentRank,
		ItemAsset->GetXpInterval(CurrentLevel),
		ItemAsset->GetLevelInterval(CurrentRank),
		ItemAsset->MaxLevel,
		NewXp,
		NewLevel,
		bDoesLevelUpdated,
		bDoesRankShouldUpdate,
		bDoesMaxLevelReached
	);

	if (!bCanLevelUp)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Level up cancelled"));
		return false;
	}

	if (!Subsystem->RemoveItem(ContainerId, EnhanceGuid, 1))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove enhance record"));
		return false;
	}

	bool bResult = Subsystem->UpdateItem(ContainerId, ItemGuid,
		[this, NewXp, NewLevel](FInventoryRecord* Record)
		{
			if (!Record) return false;
			Record->EnhanceRecord.Experience = NewXp;
			Record->EnhanceRecord.Level = NewLevel;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Level up successful"));
	return true;
}

bool UEnhanceItemSubsystem::HandleRankUp(UInventorySubsystem* Subsystem, FName ContainerId, FName ItemGuid, const FInventoryRecord* ItemRecord, UEnhanceableAsset* ItemAsset, const TMap<FName, int>& ItemQuantities)
{
	int CurrentXp = ItemRecord->EnhanceRecord.Experience;
	int CurrentLevel = ItemRecord->EnhanceRecord.Level;
	int CurrentRank = ItemRecord->EnhanceRecord.Rank;
	int LevelInterval = ItemAsset->GetLevelInterval(CurrentRank);

	PRINT_INFO(LogTemp, 5.0f, TEXT("Level Interval: %d"), LevelInterval);

	if (!UEnhanceLibrary::CanRankUp(CurrentXp, CurrentLevel, LevelInterval, CurrentRank))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Cannot rank up"));
		return false;
	}

	if (!Subsystem->RemoveItems(ContainerId, ItemQuantities))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to remove rank costs"));
		return false;
	}

	bool bResult = Subsystem->UpdateItem(ContainerId, ItemGuid,
		[this](FInventoryRecord* Record)
		{
			if (!Record) return false;
			Record->EnhanceRecord.Rank++;
			return true;
		}
	);

	if (!bResult)
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Failed to update enhanceable record"));
		return false;
	}

	PRINT_INFO(LogTemp, 1.0f, TEXT("Rank up successful"));
	return true;
}



void UEnhanceItemSubsystem::HandleGameInitialized()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	UInventorySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UInventorySubsystem>();
	if (!IsValid(Subsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("InventorySubsystem is invalid"));
		return;
	}

	InventorySubsystem = Subsystem;
}

bool UEnhanceItemSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UEnhanceItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem Initialized"));

	if (!FLatentDelegates::OnPreGameInitialized.IsBoundToObject(this))
	{
		FLatentDelegates::OnPreGameInitialized.AddUObject(this, &UEnhanceItemSubsystem::HandleGameInitialized);
	}
}

void UEnhanceItemSubsystem::Deinitialize()
{
	FLatentDelegates::OnPreGameInitialized.RemoveAll(this);

	InventorySubsystem.Reset();

	LOG_WARNING(LogTemp, TEXT("EnhanceItemSubsystem Deinitialized"));
	Super::Deinitialize();
}

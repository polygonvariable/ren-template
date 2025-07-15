// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Enhance/EnhanceRecordSubsystem.h"

// Project Headers
#include "RenAsset/Public/Inventory/Asset/Category/EnhanceableAsset.h"
#include "RenAsset/Public/Inventory/Asset/Type/EnhanceAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"
#include "RenCore/Public/Library/EnhanceLibrary.h"
#include "RenCore/Public/Record/InventoryRecord.h"
#include "RenCore/Public/Macro/GameInstanceMacro.h"
#include "RenCore/Public/Macro/LogMacro.h"


bool UEnhanceRecordSubsystem::LevelUpRecord_Implementation(const FName EnhanceableRecordId, const FName EnhanceRecordId)
{/*
	bool bFound = false;
	UInventoryAsset* REnhanceableAsset = nullptr;
	FInventoryRecord EnhanceableRecord = InventorySubsystem->GetRecordWithAsset(EnhanceableRecordId, REnhanceableAsset, bFound);
	if (!bFound || !REnhanceableAsset)
	{
		LOG_ERROR(LogTemp, "Enhanceable record not found or invalid asset");
		return false;
	}

	bFound = false;
	UInventoryAsset* REnhanceAsset = nullptr;
	FInventoryRecord EnhanceRecord = InventorySubsystem->GetRecordWithAsset(EnhanceRecordId, REnhanceAsset, bFound);
	if (!bFound || !REnhanceAsset)
	{
		LOG_ERROR(LogTemp, "Enhance record not found or invalid asset");
		return false;
	}

	UEnhanceableAsset* EnhanceableAsset = Cast<UEnhanceableAsset>(REnhanceableAsset);
	if (!EnhanceableAsset)
	{
		LOG_ERROR(LogTemp, "Inventory record is not of type enhanceable");
		return false;
	}

	UEnhanceAsset* EnhanceAsset = Cast<UEnhanceAsset>(REnhanceAsset);
	if (EnhanceAsset)
	{
		if (EnhanceableAsset->EnhanceCosts.Contains(EnhanceAsset))
		{
			int EnhancePoint = EnhanceAsset->EnhancePoints;
			return HandleLevelUp(EnhanceableRecordId, EnhanceRecordId, EnhanceableRecord, EnhanceableAsset, EnhancePoint);
		}
		LOG_ERROR(LogTemp, "Internal enhance failed, trying to check for external enhance");
	}

	int* EnhancePoints = EnhanceableAsset->EnhanceExternalCosts.Find(EnhanceAsset->ItemType);
	if (!EnhancePoints)
	{
		LOG_ERROR(LogTemp, "External enhance failed and no points found");
		return false;
	}

	return HandleLevelUp(EnhanceableRecordId, EnhanceRecordId, EnhanceableRecord, EnhanceableAsset, *EnhancePoints);*/
	return false;
}

bool UEnhanceRecordSubsystem::RankUpRecord_Implementation(const FName EnhanceableRecordId)
{
	/*
	bool bFound = false;
	UInventoryAsset* REnhanceableAsset = nullptr;
	FInventoryRecord EnhanceableRecord = InventorySubsystem->GetRecordWithAsset(EnhanceableRecordId, REnhanceableAsset, bFound);
	if (!bFound || !REnhanceableAsset)
	{
		LOG_ERROR(LogTemp, "Enhanceable record not found or invalid asset");
		return false;
	}

	UEnhanceableAsset* EnhanceableAsset = Cast<UEnhanceableAsset>(REnhanceableAsset);
	if (!EnhanceableAsset)
	{
		LOG_ERROR(LogTemp, "Inventory record is not of type enhanceable");
		return false;
	}

	TMap<int, FInventoryAssetQuantity>& EnhanceRankings = EnhanceableAsset->EnhanceRankings;
	FInventoryAssetQuantity* EnhanceRanking = EnhanceRankings.Find(EnhanceableRecord.EnhanceRecord.Rank);
	if (!EnhanceRanking) {
		LOG_ERROR(LogTemp, "Enhance rank not found");
		return false;
	}

	const TMap<FName, int> RankCostsIds;// = EnhanceRanking->ConvertToIds();
	if (!InventorySubsystem->RemoveRecords(RankCostsIds))
	{
		LOG_ERROR(LogTemp, "Failed to remove rank costs");
		return false;
	}

	EnhanceableRecord.EnhanceRecord.Rank++;
	if (!InventorySubsystem->UpdateRecord(EnhanceableRecordId, EnhanceableRecord))
	{
		LOG_ERROR(LogTemp, "Failed to update enhanceable record");
		return false;
	}

	LOG_WARNING(LogTemp, "Rank up successful");*/

	return true;
}

bool UEnhanceRecordSubsystem::HandleLevelUp(const FName& EnhanceableRecordId, const FName& EnhanceRecordId, FInventoryRecord EnhanceableRecord, UEnhanceableAsset* EnhanceableAsset, int EnhancePoint)
{
	/*
	int NewXp = 0;
	int NewLevel = 0;
	bool bDoesLevelUpdated = false;
	bool bDoesRankShouldUpdate = false;
	bool bDoesMaxLevelReached = false;

	bool bCanLevelUp = UEnhanceLibrary::CalculateEnhance(
		EnhancePoint,
		EnhanceableRecord.EnhanceRecord.Experience,
		EnhanceableRecord.EnhanceRecord.Level,
		EnhanceableRecord.EnhanceRecord.Rank,
		EnhanceableAsset->EnhanceXpInterval,
		EnhanceableAsset->EnhanceLevelInterval,
		EnhanceableAsset->EnhanceLevelMax,
		NewXp,
		NewLevel,
		bDoesLevelUpdated,
		bDoesRankShouldUpdate,
		bDoesMaxLevelReached
	);

	if (!bCanLevelUp)
	{
		LOG_ERROR(LogTemp, "Level up cancelled");
		return false;
	}

	EnhanceableRecord.EnhanceRecord.Experience = NewXp;
	EnhanceableRecord.EnhanceRecord.Level = NewLevel;

	if (!InventorySubsystem->RemoveRecord(EnhanceRecordId, 1))
	{
		LOG_ERROR(LogTemp, "Failed to remove enhance record");
		return false;
	}

	if (InventorySubsystem->UpdateRecord(EnhanceableRecordId, EnhanceableRecord))
	{
		LOG_WARNING(LogTemp, "Level up success, new xp: %s, new level: %s", *FString::FromInt(EnhanceableRecord.EnhanceRecord.Experience), *FString::FromInt(EnhanceableRecord.EnhanceRecord.Level));
		return true;
	}

	LOG_ERROR(LogTemp, "Level up failed");*/
	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryItemRarity.h"
#include "RenCoreInventory/Public/InventoryItemType.h"

#include "RenAsset/Public/Inventory/Asset/Category/CraftableAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Forward Declarations
class UEnhanceAsset;



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEnhanceableAsset : public UCraftableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int XpInterval = 5000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	TSet<TObjectPtr<UEnhanceAsset>> EnhanceCosts;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	TMap<TEnumAsByte<EInventoryItemType>, int> ExternalCosts;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	TArray<FInventoryAssetQuantity> RankingCosts;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	FCurveTableRowHandle XpCurveInterval;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	FCurveTableRowHandle LevelCurveInterval;

public:

	int GetXpInterval(int Level) const
	{
		FString Context = XpCurveInterval.RowName.ToString();
		int Value = XpCurveInterval.IsValid(Context) ? XpCurveInterval.GetCurve(Context)->Eval(Level) : XpInterval;
		return FMath::Max(1, Value);
	}

	int GetLevelInterval(int Rank) const
	{
		FString Context = LevelCurveInterval.RowName.ToString();
		int Value = LevelCurveInterval.IsValid(Context) ? LevelCurveInterval.GetCurve(Context)->Eval(Rank) : LevelInterval;
		return FMath::Max(1, Value);
	}

};


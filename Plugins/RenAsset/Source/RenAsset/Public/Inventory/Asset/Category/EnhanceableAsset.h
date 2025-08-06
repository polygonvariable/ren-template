// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"

#include "RenAsset/Public/Inventory/Asset/Category/CraftableAsset.h"
#include "RenAsset/Public/Inventory/Type/InventoryAssetQuantity.h"

// Generated Headers
#include "EnhanceableAsset.generated.h"

// Forward Declarations
class UEnhanceAsset;



/**
 * 
 */
UCLASS(Abstract)
class RENASSET_API UEnhanceableAsset : public UCraftableAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int XpInterval = 5000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int LevelInterval = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int MaxLevel = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	int MaxRank = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TSet<TObjectPtr<UEnhanceAsset>> EnhanceCosts;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TMap<TEnumAsByte<EInventoryItemType>, int> ExternalCosts;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	TMap<int, FInventoryAssetQuantity> RankingCosts;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	FCurveTableRowHandle XpCurveInterval;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item Enhanceable")
	FCurveTableRowHandle LevelCurveInterval;

public:

	int GetXpInterval(int Level) const
	{
		FString Context = XpCurveInterval.RowName.ToString();
		return XpCurveInterval.IsValid(Context) ? XpCurveInterval.GetCurve(Context)->Eval(Level) : XpInterval;
	}

	int GetLevelInterval(int Rank) const
	{
		FString Context = LevelCurveInterval.RowName.ToString();
		return LevelCurveInterval.IsValid(Context) ? LevelCurveInterval.GetCurve(Context)->Eval(Rank) : LevelInterval;
	}

};


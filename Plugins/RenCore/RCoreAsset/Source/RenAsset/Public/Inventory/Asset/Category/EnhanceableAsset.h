// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/FrameworkObjectVersion.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RCoreInventory/Public/InventoryItemRarity.h"
#include "RCoreInventory/Public/InventoryItemType.h"

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

	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	TMap<FName, int> TestMap;

	TMultiMap<FName, int> TestMultiMap;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int ExperienceInterval = 5000;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int LevelInterval = 10;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int MaxLevel = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item", Meta = (ClampMin = 1))
	int MaxRank = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	FExchangeRule EnhanceRules;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	TArray<FExchangeRule> RankingRules;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	FCurveTableRowHandle ExperienceCurve;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	FCurveTableRowHandle LevelCurve;

public:

	int GetExperienceInterval(int Level) const
	{
		FString Context = ExperienceCurve.RowName.ToString();
		int Value = ExperienceCurve.IsValid(Context) ? ExperienceCurve.GetCurve(Context)->Eval(Level) : ExperienceInterval;
		return FMath::Max(1, Value);
	}

	int GetLevelInterval(int Rank) const
	{
		FString Context = LevelCurve.RowName.ToString();
		int Value = LevelCurve.IsValid(Context) ? LevelCurve.GetCurve(Context)->Eval(Rank) : LevelInterval;
		return FMath::Max(1, Value);
	}

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Enhanceable Item")
	int TestIntUI = 0;

	int TestInt = 0;

	virtual void Serialize(FArchive& Ar) override
	{
		Super::Serialize(Ar);
		Ar.UsingCustomVersion(FFrameworkObjectVersion::GUID);
	}

};


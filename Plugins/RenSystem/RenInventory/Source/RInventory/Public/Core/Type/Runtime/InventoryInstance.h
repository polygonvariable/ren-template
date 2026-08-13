// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/AscensionData.h"

// Generated Headers
#include "InventoryInstance.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FInventoryInstance
{

	GENERATED_BODY()

public:

	FInventoryInstance() {}
	FInventoryInstance(const FGuid& InItemId, int InQuantity) : ItemId(InItemId), Quantity(InQuantity) {}


	UPROPERTY(EditAnywhere, SaveGame)
	FGuid ItemId;

	UPROPERTY(EditAnywhere, SaveGame)
	int Quantity = 0;

	UPROPERTY(EditAnywhere, SaveGame)
	FAscensionData Ascension;


	bool IsValid() const
	{
		return ItemId.IsValid() && Quantity > 0 && Ascension.IsValid();
	}

	void Reset()
	{
		ItemId.Invalidate();
		Quantity = 0;
		Ascension.Reset();
	}

	void Sanitize()
	{
		Quantity = FMath::Max(0, Quantity);
		Ascension.Sanitize();
	}

	friend inline bool operator == (const FInventoryInstance& A, const FInventoryInstance& B)
	{
		return A.ItemId == B.ItemId && A.Quantity == B.Quantity;
	}

	friend inline uint32 GetTypeHash(const FInventoryInstance& Record)
	{
		return HashCombine(GetTypeHash(Record.ItemId), GetTypeHash(Record.Quantity));
	}

	FString ToString() const
	{
		FString Detail = TEXT("Item Id: ") + ItemId.ToString();
		Detail += TEXT("\nQuantity: ") + FString::FromInt(Quantity);
		Detail += TEXT("\nAscension: ") + Ascension.ToString();
		return Detail;
	}

};


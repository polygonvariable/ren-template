// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/InventoryEntry.h"

// Engine Headers

// Project Headers
#include "Core/Type/Runtime/InventoryInstance.h"



FGuid UInventoryEntry::GetAssetInstanceId() const
{
	if (Item)
	{
		return Item->ItemId;
	}
	return FGuid();
}

void UInventoryEntry::ResetData()
{
	Quantity = 0;
	Item = nullptr;
	bIsLinked = false;
	Super::ResetData();
}



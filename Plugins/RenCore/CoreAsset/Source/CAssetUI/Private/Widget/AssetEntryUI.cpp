// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetEntryUI.h"

// Engine Headers

// Project Headers
#include "Widget/AssetEntry.h"
#include "Widget/Drag/AssetDragOperation.h"



void UAssetEntryUI::ResetDetail()
{
	CancelInitialization();
}

void UAssetEntryUI::GetAssetSubDetail(FInstancedStruct& SubDetail) const
{
	const UAssetEntry* Entry = GetListItem<UAssetEntry>();
	if (IsValid(Entry))
	{
		SubDetail = Entry->AssetSubDetail;
	}
}

void UAssetEntryUI::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	CancelInitialization();

	const UAssetEntry* Entry = Cast<UAssetEntry>(ListItemObject);
	InitializeAssetByEntry(Entry);
	InitializeEntryDetail(Entry);

	Execute_OnListItemObjectSet(this, ListItemObject);
}

void UAssetEntryUI::NativeOnItemSelectionChanged(bool bSelected)
{
	Execute_BP_OnItemSelectionChanged(this, bSelected);
}

void UAssetEntryUI::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!DragOperationClass)
	{
		return;
	}

	const UAssetEntry* Entry = GetListItem<UAssetEntry>();
	UAssetDragOperation* DragOperation = NewObject<UAssetDragOperation>(this, DragOperationClass);
	if (IsValid(DragOperation) && IsValid(Entry))
	{
		DragOperation->AssetId = Entry->AssetId;
		DragOperation->AssetInstanceId = Entry->GetAssetInstanceId();
		DragOperation->DataAsset = GetActiveAsset();
		OutOperation = DragOperation;
	}
}


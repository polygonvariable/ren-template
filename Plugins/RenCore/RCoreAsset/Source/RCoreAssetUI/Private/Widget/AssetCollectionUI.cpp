// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Widget/AssetCollectionUI.h"

// Engine Headers
#include "Components/ListView.h"

// Project Headers
#include "Filter/FilterGroup.h"
#include "Log/LogMacro.h"
#include "Widget/AssetEntry.h"



void UAssetCollectionUI::InitializeCollection()
{

}

void UAssetCollectionUI::DisplayEntries()
{

}

void UAssetCollectionUI::ClearEntries(bool bRegenerate)
{
	EntryList->ClearSelection();

	const TArray<UObject*>& Items = EntryList->GetListItems();
	for (UObject* Item : Items)
	{
		UAssetEntry* Entry = Cast<UAssetEntry>(Item);
		if (!IsValid(Entry))
		{
			continue;
		}
		Entry->ResetData();
		ReturnEntryToPool(Entry);
	}

	EntryList->ClearListItems();

	if (bRegenerate)
	{
		EntryList->RegenerateAllEntries();
	}
}

void UAssetCollectionUI::RefreshEntries()
{
	if (bAutoSelectAfterRefresh)
	{
		AutoSelectCaching();
	}

	ClearEntries(true);
	DisplayEntries();
}

UAssetEntry* UAssetCollectionUI::GetSelectedEntry()
{
	return EntryList->GetSelectedItem<UAssetEntry>();
}



void UAssetCollectionUI::AddSubDetails(const FPrimaryAssetId& Id, const FInstancedStruct& Detail)
{
	SubDetails.Add(Id, Detail);
}

void UAssetCollectionUI::RemoveSubDetails(const FPrimaryAssetId& Id)
{
	SubDetails.Remove(Id);
}

void UAssetCollectionUI::ClearSubDetails()
{
	SubDetails.Empty();
}



UFilterCriterion* UAssetCollectionUI::GetCriterionByName(FName Name) const
{
	if (!IsValid(FilterRule))
	{
		return nullptr;
	}
	return FilterRule->GetCriterionByName(Name);
}

const UFilterCriterion* UAssetCollectionUI::GetFilterRoot() const
{
	UFilterCriterion* CriterionRoot = nullptr;
	if (IsValid(FilterRule))
	{
		return FilterRule->CriterionRoot;
	}
	return CriterionRoot;
}



void UAssetCollectionUI::AutoSelectCaching()
{
	UAssetEntry* Entry = GetSelectedEntry();
	if (IsValid(Entry))
	{
		_SelectedAssetId = Entry->AssetId;
	}
}

bool UAssetCollectionUI::AutoSelectCondition(UAssetEntry* Item) const
{
	return Item->AssetId == _SelectedAssetId;
}



void UAssetCollectionUI::AddEntry(const FPrimaryAssetId& AssetId, UAssetEntry* Entry)
{
	if (!IsValid(Entry))
	{
		return;
	}

	FInstancedStruct* Detail = SubDetails.Find(AssetId);
	if (Detail != nullptr)
	{
		Entry->AssetSubDetail = *Detail;
	}

	Entry->AssetId = AssetId;
	EntryList->AddItem(Entry);

	if (bAutoSelectAfterRefresh)
	{
		if (AutoSelectCondition(Entry))
		{
			EntryList->SetSelectedItem(Entry);
		}
	}
}

void UAssetCollectionUI::ReturnEntryToPool(UAssetEntry* Item)
{
	if (IsValid(Item))
	{
		EntryPool.Add(Item);
	}
}

UAssetEntry* UAssetCollectionUI::GetEntryFromPool(const TSubclassOf<UAssetEntry>& EntryClass)
{
	if (EntryPool.Num() > 0)
	{
		return EntryPool.Pop();
	}
	return NewObject<UAssetEntry>(this, EntryClass);
}



void UAssetCollectionUI::HandleOnItemSelectionChanged(UObject* Object)
{
	const UAssetEntry* Entry = Cast<UAssetEntry>(Object);
	if (IsValid(Entry))
	{
		if (Entry->AssetId == _SelectedAssetId)
		{
			PRINT_WARNING(LogTemp, 2.0f, TEXT("already selected"));
			return;
		}

		_SelectedAssetId = Entry->AssetId;
		OnSelectionChanged.ExecuteIfBound(Entry);
	}
	else
	{
		if (bAutoClearSelection)
		{
			_SelectedAssetId = FPrimaryAssetId();
			OnSelectionCleared.ExecuteIfBound();
		}
	}
}

void UAssetCollectionUI::NativeConstruct()
{
	EntryList->OnItemSelectionChanged().AddUObject(this, &UAssetCollectionUI::HandleOnItemSelectionChanged);

	Super::NativeConstruct();
}

void UAssetCollectionUI::NativeDestruct()
{
	ClearEntries(false);

	EntryList->OnItemSelectionChanged().RemoveAll(this);

	Super::NativeDestruct();
}


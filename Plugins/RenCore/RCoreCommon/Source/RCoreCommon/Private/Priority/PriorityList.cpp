// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Priority/PriorityList.h"

// Project Headers
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"



bool UPriorityList::AddItem(UObject* Item, int Priority)
{
	if (!IsValid(Item) || Priority < 0)
	{
		LOG_ERROR(LogPriorityList, TEXT("Item is invalid or priority is less than 0"));
		return false;
	}
	
	TWeakObjectPtr<UObject>* FoundItem = Items.Find(Priority);
	if (FoundItem)
	{
		TWeakObjectPtr<UObject> WeakItem = FoundItem->Get();
		UObject* Object = WeakItem.Get();
		if (IsValid(Object))
		{
			if (Object == Item)
			{
				LOG_WARNING(LogPriorityList, TEXT("Item already exists at priority"));
				return false;
			}

			Delegates.OnRemoved.ExecuteIfBound(Object, true);
		}
	}

	Items.Add(Priority, Item);
	Delegates.OnAdded.ExecuteIfBound(Item);

	if (Priority >= HighestPriority)
	{
		HighestPriority = Priority;
		Delegates.OnChanged.ExecuteIfBound(Item);

		return true;
	}

	LOG_WARNING(LogPriorityList, TEXT("Priority is less than highest priority, added in queue"));
	return true;
}

bool UPriorityList::RemoveItem(int Priority)
{
	TWeakObjectPtr<UObject> RemovedItem;
	if (!Items.RemoveAndCopyValue(Priority, RemovedItem))
	{
		LOG_ERROR(LogPriorityList, TEXT("Item not found at priority"));
		return false;
	}

	UObject* Item = RemovedItem.Get();
	if (IsValid(Item))
	{
		Delegates.OnRemoved.ExecuteIfBound(Item, false);
	}

	if (Priority == HighestPriority)
	{
		if (Items.Num() == 0)
		{
			HighestPriority = -1;
			Delegates.OnCleared.ExecuteIfBound();
		}
		else
		{
			UpdateHighestPriority();

			TWeakObjectPtr<UObject>* FoundItem = Items.Find(HighestPriority);
			if (FoundItem)
			{
				TWeakObjectPtr<UObject> WeakItem = FoundItem->Get();
				UObject* NewItem = WeakItem.Get();
				if (IsValid(NewItem))
				{
					Delegates.OnChanged.ExecuteIfBound(NewItem);
				}
			}
		}
	}

	return true;
}

void UPriorityList::UpdateHighestPriority()
{
	int NewPriority = TNumericLimits<int>::Lowest();
	for (const TPair<int, TWeakObjectPtr<UObject>>& Kv : Items)
	{
		if (Kv.Key > NewPriority)
		{
			NewPriority = Kv.Key;
		}
	}
	
	HighestPriority = NewPriority;
}

void UPriorityList::CleanUpItems()
{
	Items.Empty();
}

FPriorityListDelegates& UPriorityList::GetPriorityDelegates()
{
	return Delegates;
}


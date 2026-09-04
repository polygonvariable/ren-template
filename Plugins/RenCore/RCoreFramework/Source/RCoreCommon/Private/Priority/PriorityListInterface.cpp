// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Priority/PriorityListInterface.h"


bool IPriorityListInterface::AddPriorityItem(UObject* Item, int Priority)
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();
	int& HighestPriority = GetHighestPriority();

	if (!IsValid(Item) || Priority < 0)
	{
		return false;
	}
	
	TWeakObjectPtr<UObject>* FoundItem = PriorityItems.Find(Priority);
	if (FoundItem)
	{
		TWeakObjectPtr<UObject> WeakItem = FoundItem->Get();
		UObject* Object = WeakItem.Get();
		if (IsValid(Object))
		{
			if (Object == Item)
			{
				return false;
			}
			OnPriorityItemRemoved(Object, true);
		}
	}

	PriorityItems.Add(Priority, Item);
	OnPriorityItemAdded(Item);

	if (Priority >= HighestPriority)
	{
		HighestPriority = Priority;
		OnPriorityItemChanged(Item);
	}

	return true;
}

bool IPriorityListInterface::RemovePriorityItem(int Priority)
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();
	int& HighestPriority = GetHighestPriority();

	TWeakObjectPtr<UObject> RemovedItem;
	if (!PriorityItems.RemoveAndCopyValue(Priority, RemovedItem))
	{
		return false;
	}

	UObject* Item = RemovedItem.Get();
	if (IsValid(Item))
	{
		OnPriorityItemRemoved(Item, false);
	}

	if (Priority == HighestPriority)
	{
		if (PriorityItems.Num() == 0)
		{
			HighestPriority = -1;
			OnPriorityItemCleared();
		}
		else
		{
			UpdateHighestPriority();

			TWeakObjectPtr<UObject>* FoundItem = PriorityItems.Find(HighestPriority);
			if (FoundItem)
			{
				TWeakObjectPtr<UObject> WeakItem = FoundItem->Get();
				UObject* NewItem = WeakItem.Get();
				if (IsValid(NewItem))
				{
					OnPriorityItemChanged(NewItem);
				}
			}
		}
	}

	return true;
}

void IPriorityListInterface::ClearPriorityItems()
{
	GetPriorityItems().Empty();
}

void IPriorityListInterface::UpdateHighestPriority()
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();
	int& HighestPriority = GetHighestPriority();

	int NewPriority = TNumericLimits<int>::Lowest();
	for (const TPair<int, TWeakObjectPtr<UObject>>& Kv : PriorityItems)
	{
		if (Kv.Key > NewPriority)
		{
			NewPriority = Kv.Key;
		}
	}

	HighestPriority = NewPriority;
}


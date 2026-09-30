// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Priority/PriorityListInterface.h"


bool IPriorityListInterface::AddPriorityItem(UObject* Item, int Priority)
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();
	if (!IsValid(Item) || Priority < 0)
	{
		return false;
	}

	UObject* PreviousItem = nullptr;
	if (HighestPriority >= 0)
	{
		TWeakObjectPtr<UObject>* FoundHighestItem = PriorityItems.Find(HighestPriority);
		if (FoundHighestItem)
		{
			PreviousItem = FoundHighestItem->Get();
		}
	}

	TWeakObjectPtr<UObject>* FoundItem = PriorityItems.Find(Priority);
	if (FoundItem)
	{
		UObject* Object = FoundItem->Get();
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
		OnPriorityItemChanged(PreviousItem, Item);
	}

	return true;
}

bool IPriorityListInterface::RemovePriorityItem(int Priority)
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();

	TWeakObjectPtr<UObject> RemovedItem;
	if (!PriorityItems.RemoveAndCopyValue(Priority, RemovedItem))
	{
		return false;
	}

	if (Priority == HighestPriority)
	{
		UObject* Item = RemovedItem.Get();
		OnPriorityItemRemoved(Item, false);

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
				UObject* NewItem = FoundItem->Get();
				if (IsValid(NewItem))
				{
					OnPriorityItemChanged(Item, NewItem);
				}
			}
		}
	}

	return true;
}

void IPriorityListInterface::ClearPriorityItems()
{
	HighestPriority = -1;
	GetPriorityItems().Empty();
	OnPriorityItemCleared();
}

int IPriorityListInterface::GetHighestPriority() const
{
	return HighestPriority;
}

void IPriorityListInterface::UpdateHighestPriority()
{
	TMap<int, TWeakObjectPtr<UObject>>& PriorityItems = GetPriorityItems();

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


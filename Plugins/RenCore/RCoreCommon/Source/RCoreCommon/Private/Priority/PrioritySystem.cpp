// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Priority/PrioritySystem.h"
#include "RCoreLibrary/Public/LogMacro.h"



void UPrioritySystem::Initialize()
{
}

void UPrioritySystem::Deinitialize()
{
	CleanUpItems();
}

bool UPrioritySystem::AddItem(UObject* Item, int Priority)
{
	if (!IsValid(Item) || Priority < 0)
	{
		LOG_ERROR(LogTemp, TEXT("Item is not valid or priority is less than 0"));
		return false;
	}
	
	TWeakObjectPtr<UObject>* FoundItem = Items.Find(Priority);
	if (FoundItem)
	{
		UObject* Object = FoundItem->Get();
		if (IsValid(Object))
		{
			if (Object == Item)
			{
				LOG_WARNING(LogTemp, TEXT("Item already exists at priority"));
				return false;
			}

			OnItemRemoved(Object, true);
		}
	}

	Items.Add(Priority, Item);
	OnItemAdded(Item);

	if (Priority >= HighestPriority)
	{
		HighestPriority = Priority;
		OnItemChanged(Item);
	}
	else
	{
		LOG_WARNING(LogTemp, TEXT("Priority is less than highest priority, added in queue"));
	}
	return true;
}

bool UPrioritySystem::RemoveItem(int Priority)
{
	TWeakObjectPtr<UObject> RemovedItem;
	if (!Items.RemoveAndCopyValue(Priority, RemovedItem))
	{
		LOG_ERROR(LogTemp, TEXT("Item not found"));
		return false;
	}

	UObject* RemovedItemPtr = RemovedItem.Get();
	if (IsValid(RemovedItemPtr))
	{
		OnItemRemoved(RemovedItemPtr, false);
	}

	if (Priority == HighestPriority)
	{
		if (Items.Num() == 0)
		{
			HighestPriority = -1;
			OnNoItemsLeft();
		}
		else
		{
			CalculateHighestPriority();

			TWeakObjectPtr<UObject> NewItem = Items.FindRef(HighestPriority);
			UObject* NewItemPtr = NewItem.Get();
			if (IsValid(NewItemPtr))
			{
				OnItemChanged(NewItemPtr);
			}
		}
	}

	return true;
}

void UPrioritySystem::CalculateHighestPriority()
{
	int NewPriority = TNumericLimits<int>::Lowest();
	for (const auto& Pair : Items)
	{
		if (Pair.Key > NewPriority)
		{
			NewPriority = Pair.Key;
		}
	}
	
	HighestPriority = NewPriority;
}

void UPrioritySystem::CleanUpItems()
{
	Items.Empty();
}

#if WITH_EDITOR

FString UPrioritySystem::GetDebugString()
{
	FString Result;

	Result += FString::Printf(TEXT("Total items: %d\n"), Items.Num());
	Result += FString::Printf(TEXT("Priority\tName\n"));

	for (const TPair<int, TWeakObjectPtr<UObject>>& Pair : Items)
	{
		int Priority = Pair.Key;
		TObjectPtr<UObject> Item = Pair.Value.Get();

		FString Title = IsValid(Item) ? Item->GetName() : TEXT("???");

		Result += FString::Printf(TEXT("%d\t%s\n"), Priority, *Title);
	}

	return Result;
}

#endif

void UPrioritySystem::OnItemAdded(UObject* Item)
{
}

void UPrioritySystem::OnItemRemoved(UObject* Item, bool bWasReplaced)
{
}

void UPrioritySystem::OnItemChanged(UObject* Item)
{
}

void UPrioritySystem::OnNoItemsLeft()
{
}


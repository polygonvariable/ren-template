// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Priority/PrioritySystem.h"
#include "RCoreLibrary/Public/LogMacro.h"



void UObjectPrioritySystem::Initialize()
{
}

void UObjectPrioritySystem::Deinitialize()
{
	CleanUpItems();
}

bool UObjectPrioritySystem::AddItem(UObject* Item, int Priority)
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

			HandleItemRemoved(Object, true);
		}
	}

	Items.Add(Priority, Item);
	HandleItemAdded(Item);

	if (Priority >= HighestPriority)
	{
		HighestPriority = Priority;
		HandleItemChanged(Item);
	}
	else
	{
		LOG_WARNING(LogTemp, TEXT("Priority is less than highest priority, added in queue"));
	}
	return true;
}

bool UObjectPrioritySystem::RemoveItem(int Priority)
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
		HandleItemRemoved(RemovedItemPtr, false);
	}

	if (Priority == HighestPriority)
	{
		if (Items.Num() == 0)
		{
			HighestPriority = -1;
			HandleNoItemsLeft();
		}
		else
		{
			CalculateHighestPriority();

			TWeakObjectPtr<UObject> NewItem = Items.FindRef(HighestPriority);
			UObject* NewItemPtr = NewItem.Get();
			if (IsValid(NewItemPtr))
			{
				HandleItemChanged(NewItemPtr);
			}
		}
	}

	return true;
}

void UObjectPrioritySystem::CalculateHighestPriority()
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

void UObjectPrioritySystem::CleanUpItems()
{
	Items.Empty();
}

#if WITH_EDITOR

FString UObjectPrioritySystem::GetDebugString()
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

void UObjectPrioritySystem::HandleItemAdded(UObject* Item)
{
}

void UObjectPrioritySystem::HandleItemRemoved(UObject* Item, bool bWasReplaced)
{
}

void UObjectPrioritySystem::HandleItemChanged(UObject* Item)
{
}

void UObjectPrioritySystem::HandleNoItemsLeft()
{
}


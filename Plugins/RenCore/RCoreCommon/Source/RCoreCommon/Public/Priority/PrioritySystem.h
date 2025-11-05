// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "PrioritySystem.generated.h"



/**
 *
 */
UCLASS()
class RCORECOMMON_API UPrioritySystem : public UObject
{

	GENERATED_BODY()

public:

	virtual void Initialize();
	virtual void Deinitialize();

	bool AddItem(UObject* Item, int Priority);
	bool RemoveItem(int Priority);

	virtual void CleanUpItems();

#if WITH_EDITOR

	virtual FString GetDebugString();

#endif

protected:

	int HighestPriority = -1;

	TMap<int, TWeakObjectPtr<UObject>> Items;

	void CalculateHighestPriority();

	virtual void OnItemAdded(UObject* Item);
	virtual void OnItemRemoved(UObject* Item, bool bWasReplaced);
	virtual void OnItemChanged(UObject* Item);
	virtual void OnNoItemsLeft();

};


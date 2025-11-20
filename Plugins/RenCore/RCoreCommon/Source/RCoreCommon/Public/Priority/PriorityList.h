// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "PriorityList.generated.h"



/**
 *
 *
 *
 */
struct FPriorityListDelegates
{

public:

	DECLARE_DELEGATE_OneParam(FItemAdded, UObject*);
	FItemAdded OnAdded;

	DECLARE_DELEGATE_TwoParams(FItemRemoved, UObject*, bool /* bReplaced */);
	FItemRemoved OnRemoved;

	DECLARE_DELEGATE_OneParam(FItemChanged, UObject*);
	FItemChanged OnChanged;

	DECLARE_DELEGATE(FItemCleared);
	FItemCleared OnCleared;

	void ClearAll()
	{
		OnAdded.Unbind();
		OnRemoved.Unbind();
		OnChanged.Unbind();
		OnCleared.Unbind();
	}

};


/**
 * 
 * 
 * 
 */
UCLASS()
class RCORECOMMON_API UPriorityList : public UObject
{

	GENERATED_BODY()

public:

	bool AddItem(UObject* Item, int Priority);
	bool RemoveItem(int Priority);

	void CleanUpItems();

	FPriorityListDelegates& GetPriorityDelegates();

private:

	FPriorityListDelegates Delegates;

	int HighestPriority = -1;

	TMap<int, TWeakObjectPtr<UObject>> Items;

	void UpdateHighestPriority();

};


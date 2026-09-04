// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Generated Headers
#include "PriorityListInterface.generated.h"

// Module Macros
#define REN_API RCORECOMMON_API


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPriorityListInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class REN_API IPriorityListInterface
{

	GENERATED_BODY()

protected:

	virtual bool AddPriorityItem(UObject* Item, int Priority);
	virtual bool RemovePriorityItem(int Priority);
	virtual void ClearPriorityItems();

	virtual void OnPriorityItemAdded(UObject* Item) {};
	virtual void OnPriorityItemRemoved(UObject* Item, bool bReplaced) {};
	virtual void OnPriorityItemChanged(UObject* Item) {};
	virtual void OnPriorityItemCleared() {};

	virtual TMap<int, TWeakObjectPtr<UObject>>& GetPriorityItems() = 0;
	virtual int& GetHighestPriority() = 0;

private:

	void UpdateHighestPriority();

};

// Module Macros
#undef REN_API


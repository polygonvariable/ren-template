// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/PoolCollection.h"


/**
 *
 */
class FPoolHelper
{

public:

	template<typename T>
	static T* Acquire(TMap<UClass*, FPoolCollection>& PoolContainer, UClass* Class, UObject* Outer)
	{
		FPoolCollection& Pool = PoolContainer.FindOrAdd(Class);
		if (Pool.List.Num() > 0)
		{
			return Cast<T>(Pool.List.Pop());
		}
		return NewObject<T>(Outer, Class);
	}
	
	static void Return(TMap<UClass*, FPoolCollection>& PoolContainer, UObject* Item)
	{
		if (IsValid(Item))
		{
			UClass* Class = Item->GetClass();
			FPoolCollection& Pool = PoolContainer.FindOrAdd(Class);
			Pool.List.Push(Item);
		}
	}

};
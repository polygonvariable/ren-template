// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/PoolCollection.h"


/**
 *
 */
namespace FPoolHelper
{

	template<typename T>
	T* AcquireFromContainer(TMap<UClass*, FPoolCollection>& PoolContainer, UClass* Class, UObject* Outer)
	{
		FPoolCollection& Pool = PoolContainer.FindOrAdd(Class);
		if (Pool.List.Num() > 0)
		{
			return Cast<T>(Pool.List.Pop());
		}
		return NewObject<T>(Outer, Class);
	}

	void ReturnToContainer(TMap<UClass*, FPoolCollection>& PoolContainer, UObject* Item)
	{
		if (IsValid(Item))
		{
			UClass* Class = Item->GetClass();
			FPoolCollection& Pool = PoolContainer.FindOrAdd(Class);
			Pool.List.Push(Item);
		}
	}

	template<typename T>
	T* AcquireFromArray(TArray<TObjectPtr<T>>& PoolContainer, UClass* Class, UObject* Outer)
	{
		if (PoolContainer.Num() > 0)
		{
			return PoolContainer.Pop();
		}
		return NewObject<T>(Outer, Class);
	}

	template<typename T>
	T* AcquireWidgetFromArray(TArray<TObjectPtr<T>>& PoolContainer, UClass* Class, UUserWidget* Outer)
	{
		if (PoolContainer.Num() > 0)
		{
			return PoolContainer.Pop();
		}
		return CreateWidget<T>(Outer, Class);
	}

	template<typename T>
	void ReturnToArray(TArray<TObjectPtr<T>>& PoolContainer, T* Item)
	{
		if (IsValid(Item))
		{
			PoolContainer.Push(Item);
		}
	}

};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "UObject/Interface.h"

// Project Headers

// Generated Headers
#include "ObjectPoolInterface.generated.h"

// Forward Declarations



DECLARE_DELEGATE_OneParam(FObjectPoolReleaseHandler, UObject* /* Object */);

UINTERFACE(MinimalAPI)
class UObjectPoolInterface : public UInterface
{

	GENERATED_BODY()

};


/**
 * 
 */
class RCOREPOOL_API IObjectPoolInterface
{

	GENERATED_BODY()

public:

	FObjectPoolReleaseHandler ReleaseObjectHandler;

	virtual void ReturnToPool() = 0;

};


/**
 *
 */
UCLASS()
class RCOREPOOL_API UPersistentObjectPool : public UObject
{

	GENERATED_BODY()

public:
	
	template<class T>
	T* AcquireObject(int Index, UObject* Outer)
	{
		if (PoolObjects.IsValidIndex(Index))
		{
			return Cast<T>(PoolObjects[Index]);
		}

		T* Object = NewObject<T>(Outer, ObjectClass);
		PoolObjects.Push(Object);

		return Object;
	}

	template<class T>
	void Reset()
	{
		/*for (auto Object : PoolObjects)
		{
			if (T* CastedObject = Cast<T>(Object))
			{
				CastedObject->ReleaseObjectHandler.Unbind();
			}
		}*/
		PoolObjects.Empty();
	}

	void SetObjectClass(UClass* InObjectClass)
	{
		ObjectClass = InObjectClass;
	}

	const TArray<TObjectPtr<UObject>>& GetPoolObjects() const { return PoolObjects; }

protected:

	UClass* ObjectClass;
	TArray<TObjectPtr<UObject>> PoolObjects;

};


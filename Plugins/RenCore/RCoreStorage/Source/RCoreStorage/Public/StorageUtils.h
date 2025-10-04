// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Forward Declarations



/**
 *
 */
class RCORESTORAGE_API StorageUtils
{

public:

	static UObject* GetStorageInterface(UGameInstance* Context);

	template<class T>
	static T* GetStorageInterface(UGameInstance* Context)
	{
		return Cast<T>(GetStorageInterface(Context));
	}

};


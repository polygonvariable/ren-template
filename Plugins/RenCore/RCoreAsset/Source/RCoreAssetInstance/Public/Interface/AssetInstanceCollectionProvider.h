// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetInstanceCollectionProvider.generated.h"

// Module Macros
#define REN_API RCOREASSETINSTANCE_API

// Forward Declarations
class IAssetInstanceCollection;


UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UAssetInstanceCollectionProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class REN_API IAssetInstanceCollectionProvider
{

	GENERATED_BODY()

public:

	virtual IAssetInstanceCollection* GetInstanceCollection(const FName& CollectionId) const = 0;
	virtual FPrimaryAssetType GetSupportedAssetType() const = 0;
	virtual FName GetPrimaryCollectionId() const = 0;

	IAssetInstanceCollection* GetPrimaryCollection() const
	{
		return GetInstanceCollection(GetPrimaryCollectionId());
	}

	template<typename T>
	T* GetPrimaryCollection() const
	{
		return Cast<T>(GetPrimaryCollection());
	}

};


// Module Macros
#undef REN_API


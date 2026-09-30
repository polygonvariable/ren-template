// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "UObject/Interface.h"

// Generated Headers
#include "AssetInstanceCollectionProvider.generated.h"

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
class CASSETINSTANCE_API IAssetInstanceCollectionProvider
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


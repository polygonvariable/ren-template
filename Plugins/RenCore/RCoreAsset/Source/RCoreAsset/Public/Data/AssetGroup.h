// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "AssetGroup.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

// Forward Declarations
class UAssetCollection;
struct FInstancedStruct;


/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, EditInlineNew, CollapseCategories)
class UAssetGroup : public UObject
{

	GENERATED_BODY()

public:

	REN_API virtual const UAssetCollection* GetCollectionRule() const;
	REN_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const;

	template<typename T>
	const T* GetCollectionRule() const
	{
		return Cast<const T>(GetCollectionRule());
	}

	template<typename T>
	const T* GetCollectionRule(const FInstancedStruct& Context) const
	{
		return Cast<const T>(GetCollectionRule(Context));
	}

};


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Asset Group (Single)")
class UAssetGroup_Single : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	REN_API virtual const UAssetCollection* GetCollectionRule() const override;
	REN_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TObjectPtr<UAssetCollection> Collection = nullptr;

};


/**
 *
 */
UCLASS(MinimalAPI, DisplayName = "Asset Group (List)")
class UAssetGroup_List : public UAssetGroup
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionGroup
	REN_API virtual const UAssetCollection* GetCollectionRule() const override;
	REN_API virtual const UAssetCollection* GetCollectionRule(const FInstancedStruct& Context) const override;
	// ~ End of UAssetCollectionGroup

protected:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UAssetCollection>> Collections;

};


// Module Macros
#undef REN_API


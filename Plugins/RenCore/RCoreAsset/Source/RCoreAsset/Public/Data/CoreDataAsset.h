// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Generated Headers
#include "CoreDataAsset.generated.h"

// Module Macros
#define REN_API RCOREASSET_API

// Forward Declarations
class UAssetFragment;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCoreDataAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable)
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UAssetFragment>> Fragments;


	REN_API const UAssetFragment* FindFragmentByClass(TSubclassOf<UAssetFragment> InClass) const;

	template<typename T>
	const T* FindFragmentByClass() const
	{
		return Cast<T>(FindFragmentByClass(T::StaticClass()));
	}

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	REN_API virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset
#endif

};


// Module Macros
#undef REN_API


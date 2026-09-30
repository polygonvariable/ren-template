// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DataAsset.h"

// Generated Headers
#include "FragmentedDataAsset.generated.h"

// Forward Declarations
class UAssetFragment;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UFragmentedDataAsset : public UPrimaryDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AssetRegistrySearchable, Category = "Asset Detail")
	FText DisplayName = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asset Detail")
	FText Description = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Asset Detail")
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Fragments")
	TArray<TObjectPtr<UAssetFragment>> Fragments;


	CASSET_API const UAssetFragment* FindFragmentByClass(TSubclassOf<UAssetFragment> InClass) const;

	template<typename T>
	const T* FindFragmentByClass() const
	{
		return Cast<T>(FindFragmentByClass(T::StaticClass()));
	}

#if WITH_EDITORONLY_DATA
	// ~ UPrimaryDataAsset
	CASSET_API virtual void UpdateAssetBundleData() override;
	// ~ End of UPrimaryDataAsset
#endif

public:

	template<typename T>
	static const T* GetFragmentByClass(UPrimaryDataAsset* Asset)
	{
		UFragmentedDataAsset* CoreAsset = Cast<UFragmentedDataAsset>(Asset);
		if (!IsValid(CoreAsset))
		{
			return nullptr;
		}
		return CoreAsset->FindFragmentByClass<T>();
	}

};


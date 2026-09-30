// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/FragmentedDataAsset.h"

// Project Headers
#include "Data/AssetFragment.h"


const UAssetFragment* UFragmentedDataAsset::FindFragmentByClass(TSubclassOf<UAssetFragment> InClass) const
{
	for (const TObjectPtr<UAssetFragment>& Fragment : Fragments)
	{
		if (Fragment->IsA(InClass))
		{
			return Fragment.Get();
		}
	}
	return nullptr;
}

#if WITH_EDITORONLY_DATA
void UFragmentedDataAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (TObjectPtr<UAssetFragment>& Fragment : Fragments)
	{
		if (IsValid(Fragment))
		{
			Fragment->AppendAssetBundleData(AssetBundleData);
		}
	}
}
#endif


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WorldFragmentSettings.h"

// Project Headers
#include "WorldFragment.h"


const UWorldFragment* AWorldFragmentSettings::FindFragmentByClass(TSubclassOf<UWorldFragment> InClass) const
{
	for (const TObjectPtr<UWorldFragment>& Fragment : Fragments)
	{
		if (Fragment->IsA(InClass))
		{
			return Fragment.Get();
		}
	}
	return nullptr;
}


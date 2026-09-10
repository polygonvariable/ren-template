// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "WorldFragmentSettings.h"

// Project Headers
#include "WorldConfigAsset.h"


const UWorldConfigAsset* AWorldFragmentSettings::FindConfigByClass(TSubclassOf<UWorldConfigAsset> InClass) const
{
	for (const TObjectPtr<UWorldConfigAsset>& Item : Configs)
	{
		if (Item->IsA(InClass))
		{
			return Item.Get();
		}
	}
	return nullptr;
}


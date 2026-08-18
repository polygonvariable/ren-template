// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EquipmentDataDefinition.h"

// Project Headers
#include "Core/EquipmentSettings.h"


#if WITH_EDITOR
void UEquipmentDataDefinition::AppendAssetBundleData(FAssetBundleData& Data)
{
	const UEquipmentSettings* Settings = UEquipmentSettings::Get();
	const TArray<FName>& EquipmentBundles = Settings->EquipmentBundles;

	if (EquipmentBundles.IsValidIndex(0) && EquipmentBundles[0] != NAME_None)
	{
		const FName& BundleName = EquipmentBundles[0];
		
		Data.AddBundleAsset(BundleName, ControllerClass.ToSoftObjectPath().GetAssetPath());
		Data.AddBundleAsset(BundleName, ActorClass.ToSoftObjectPath().GetAssetPath());
		Data.AddBundleAsset(BundleName, AbilityCollection.ToSoftObjectPath().GetAssetPath());
	}
}
#endif


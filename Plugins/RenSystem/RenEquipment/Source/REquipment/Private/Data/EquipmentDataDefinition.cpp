// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Data/EquipmentDataDefinition.h"

// Engine Headers
#include "Misc/DataValidation.h"

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

EDataValidationResult UEquipmentDataDefinition::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (ControllerClass.IsNull())
	{
		Context.AddError(FText::FromString("ControllerClass is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (ActorClass.IsNull())
	{
		Context.AddError(FText::FromString("ActorClass is invalid"));
		return EDataValidationResult::Invalid;
	}

	if (AbilityCollection.IsNull())
	{
		Context.AddError(FText::FromString("AbilityCollection is invalid"));
		return EDataValidationResult::Invalid;
	}

	return Result;
}
#endif


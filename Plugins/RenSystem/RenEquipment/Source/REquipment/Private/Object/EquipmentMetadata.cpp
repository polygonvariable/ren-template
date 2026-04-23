// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Object/EquipmentMetadata.h"

// Engine Headers
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

// Project Headers
#include "Actor/EquipmentActor.h"
#include "Asset/CoreDataAsset.h"
#include "Asset/EquipmentDataAsset.h"
#include "Definition/AscensionData.h"
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Library/AssetInstanceUtil.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Settings/EquipmentSettings.h"


const UEquipmentDataDefinition* UEquipmentFragment::GetEquipmentDefinitionCDO() const
{
	if (!IsValid(EquipmentDefinition))
	{
		return nullptr;
	}
	return EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
}

#if WITH_EDITORONLY_DATA
void UEquipmentFragment::AppendAssetBundleData(FAssetBundleData& Data)
{
	if (IsValid(EquipmentDefinition))
	{
		UEquipmentDataDefinition* DefinitionCDO = EquipmentDefinition->GetDefaultObject<UEquipmentDataDefinition>();
		if (IsValid(DefinitionCDO))
		{
			DefinitionCDO->AppendAssetBundleData(Data);
		}
	}
}

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


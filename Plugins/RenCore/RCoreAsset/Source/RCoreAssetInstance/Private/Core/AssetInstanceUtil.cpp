// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Core/AssetInstanceUtil.h"

// Project Headers
#include "Core/Interface/AssetInstanceCollectionProvider.h"
#include "Core/AssetInstanceSettings.h"


IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetId& AssetId)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetInstanceCollectionProvider(Context->GetGameInstance(), AssetId.PrimaryAssetType);
}

IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetId& AssetId)
{
	return GetInstanceCollectionProvider(Context, AssetId.PrimaryAssetType);
}

IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetType& AssetType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetInstanceCollectionProvider(Context->GetGameInstance(), AssetType);
}

IAssetInstanceCollectionProvider* FAssetInstanceUtil::GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetType& AssetType)
{
	const UAssetInstanceSettings* Settings = UAssetInstanceSettings::Get();
	const TSoftClassPtr<UGameInstanceSubsystem>* FoundProvider = Settings->InstanceProviders.Find(AssetType);
	if (!FoundProvider)
	{
		return nullptr;
	}

	UClass* SubsystemClass = FoundProvider->Get();
	if (!SubsystemClass)
	{
		return nullptr;
	}

	UGameInstanceSubsystem* Subsystem = Context->GetSubsystemBase(SubsystemClass);
	IAssetInstanceCollectionProvider* CollectionProvider = Cast<IAssetInstanceCollectionProvider>(Subsystem);

	return CollectionProvider;
}






IAssetInstanceCollection* FAssetInstanceUtil::GetInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetInstanceCollection(Context->GetGameInstance(), AssetType, CollectionId);
}

IAssetInstanceCollection* FAssetInstanceUtil::GetInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId)
{
	IAssetInstanceCollectionProvider* CollectionProvider = GetInstanceCollectionProvider(Context, AssetType);
	if (!CollectionProvider)
	{
		return nullptr;
	}
	return CollectionProvider->GetInstanceCollection(CollectionId);
}






IAssetInstanceCollection* FAssetInstanceUtil::GetPrimaryInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType)
{
	if (!IsValid(Context))
	{
		return nullptr;
	}
	return GetPrimaryInstanceCollection(Context->GetGameInstance(), AssetType);
}

IAssetInstanceCollection* FAssetInstanceUtil::GetPrimaryInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType)
{
	IAssetInstanceCollectionProvider* CollectionProvider = GetInstanceCollectionProvider(Context, AssetType);
	if (!CollectionProvider)
	{
		return nullptr;
	}
	return CollectionProvider->GetPrimaryCollection();
}



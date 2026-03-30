// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Library/AssetInstanceUtil.h"

// Project Headers
#include "Interface/AssetInstanceCollectionProvider.h"
#include "Settings/AssetInstanceSettings.h"


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

	//const TArray<UGameInstanceSubsystem*>& Subsystems = Context->GetSubsystemArray<UGameInstanceSubsystem>();

	//for (UGameInstanceSubsystem* Subsystem : Subsystems)
	//{
	//	IAssetInstanceCollectionProvider* TransactionInterface = Cast<IAssetInstanceCollectionProvider>(Subsystem);
	//	if (TransactionInterface)
	//	{
	//		if (TransactionInterface->GetSupportedAssetType() == HandleType)
	//		{
	//			return TransactionInterface;
	//		}
	//	}
	//}

	//return nullptr;
}


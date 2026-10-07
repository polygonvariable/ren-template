// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "EventflowLibrary.h"

// Project Headers
#include "EventflowEngineProvider.h"
#include "EventflowSettings.h"


IEventflowEngineProvider* FEventflowLibrary::GetEngineProvider(UWorld* Context, const FPrimaryAssetId& AssetId)
{
	return GetEngineProvider(Context, AssetId.PrimaryAssetType);
}

IEventflowEngineProvider* FEventflowLibrary::GetEngineProvider(UWorld* Context, const FPrimaryAssetType& AssetType)
{
	const UEventflowSettings* Settings = UEventflowSettings::Get();
	const TSoftClassPtr<USubsystem>* FoundProvider = Settings->EngineProviders.Find(AssetType);
	if (!FoundProvider)
	{
		return nullptr;
	}

	UClass* SubsystemClass = FoundProvider->Get();
	if (!SubsystemClass)
	{
		return nullptr;
	}

	UWorldSubsystem* Subsystem = Context->GetSubsystemBase(SubsystemClass);
	IEventflowEngineProvider* CollectionProvider = Cast<IEventflowEngineProvider>(Subsystem);

	return CollectionProvider;
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RenGameInstance.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreSettings/Public/GameMetadataSettings.h"



void URenGameInstance::Init()
{
	Super::Init();
	PreloadAssets();
}

void URenGameInstance::GameInit()
{
	FLatentDelegates::OnPreGameInitialized.Broadcast();
}

void URenGameInstance::PreloadAssets()
{
	const UGameMetadataSettings* GameMetadata = GetDefault<UGameMetadataSettings>();
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();

	if (!GameMetadata || !AssetManager)
	{
		return;
	}

	const TArray<FPrimaryAssetId>& PreloadAssets = GameMetadata->PreloadAssets;
	if (PreloadAssets.Num() == 0)
	{
		GameInit();
		return;
	}

	TWeakObjectPtr<URenGameInstance> WeakThis(this);

	TFunction<void(bool)> AsyncCallback = [WeakThis](bool bSuccess)
		{
			URenGameInstance* Owner = WeakThis.Get();
			if (!Owner || !bSuccess)
			{
				return;
			}
			Owner->GameInit();
		};

	AssetManagerUtils::LoadPrimaryAssets(this, AssetManager, PreloadAssets, AsyncCallback);
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RGameInstance.h"

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreDelegate/Public/LatentDelegates.h"
#include "RCoreLibrary/Public/AssetManagerUtils.h"
#include "RCoreSettings/Public/GameMetadataSettings.h"



void URGameInstance::Init()
{
	Super::Init();
	PreloadAssets();
}

void URGameInstance::GameInit()
{
	FLatentDelegates::OnPreGameInitialized.Broadcast();
}

void URGameInstance::PreloadAssets()
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

	TWeakObjectPtr<URGameInstance> WeakThis(this);

	TFunction<void(bool)> AsyncCallback = [WeakThis](bool bSuccess)
		{
			URGameInstance* Owner = WeakThis.Get();
			if (!Owner || !bSuccess)
			{
				return;
			}
			Owner->GameInit();
		};

	AssetManagerUtils::LoadPrimaryAssets(this, AssetManager, PreloadAssets, AsyncCallback);
}


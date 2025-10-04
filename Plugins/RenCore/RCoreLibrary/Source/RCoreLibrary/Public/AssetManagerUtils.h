// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

// Forward Declarations
class UAssetManager;



/**
 *
 */
class AssetManagerUtils
{

public:

	template <typename TReturnType>
	static TReturnType GetAssetTagValue(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, FName Tag);

	RCORELIBRARY_API static void LoadPrimaryAsset(UObject* Outer, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded);
    RCORELIBRARY_API static void LoadPrimaryAsset(UObject* Outer, UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded);

	RCORELIBRARY_API static void LoadPrimaryAssets(UObject* Outer, UAssetManager* AssetManager, TArray<FPrimaryAssetId> AssetIds, TFunction<void(bool)> OnLoaded);

    template <typename CallbackType, typename... AssetType>
	static void LoadPrimaryAssetsArgs(UObject* Outer, UAssetManager* AssetManager, CallbackType&& OnLoaded, AssetType... AssetIds)
	{
		constexpr size_t Size = sizeof...(AssetIds);
		TArray<FPrimaryAssetId> PrimaryAssetIds = { AssetIds... };

		if (!IsValid(AssetManager) || !IsValid(Outer))
		{
			ForwardAsyncCallback(OnLoaded, false, PrimaryAssetIds, nullptr, std::make_index_sequence<Size>{});
			return;
		}

		AssetManager->LoadPrimaryAssets(PrimaryAssetIds, TArray<FName>(),
			FStreamableDelegate::CreateWeakLambda(
				Outer,
				[AssetManager, PrimaryAssetIds, Size, Callback = MoveTemp(OnLoaded)]()
				{
					if (!IsValid(AssetManager))
					{
						ForwardAsyncCallback(Callback, false, PrimaryAssetIds, nullptr, std::make_index_sequence<Size>{});
						return;
					}
					ForwardAsyncCallback(Callback, true, PrimaryAssetIds, AssetManager, std::make_index_sequence<Size>{});
				}
			)
		);
	}

protected:

    template <typename CallbackType, size_t... I>
	static void ForwardAsyncCallback(CallbackType& Callback, bool bSuccess, const TArray<FPrimaryAssetId>& AssetIds, UAssetManager* AssetManager, std::index_sequence<I...>)
	{
		TArray<UObject*> LoadedObjects;
		LoadedObjects.Reserve(AssetIds.Num());

		if (IsValid(AssetManager))
		{
			for (const FPrimaryAssetId& AssetId : AssetIds)
			{
				UObject* LoadedObject = AssetManager->GetPrimaryAssetObject(AssetId);
				LoadedObjects.Add(LoadedObject);
			}
		}

		Callback(bSuccess, LoadedObjects[I]...);
	}

};


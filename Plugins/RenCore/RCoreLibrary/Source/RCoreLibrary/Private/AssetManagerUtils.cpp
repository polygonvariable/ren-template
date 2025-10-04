// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AssetManagerUtils.h"

// Engine Headers

// Project Headers



template <typename TReturnType>
TReturnType AssetManagerUtils::GetAssetTagValue(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, FName Tag)
{
	if (!IsValid(AssetManager))
	{
		return TReturnType{};
	}

	TReturnType ReturnData{};
	FAssetData AssetData;
	if (AssetManager->GetPrimaryAssetData(AssetId, AssetData))
	{
		AssetData.GetTagValue<TReturnType>(Tag, ReturnData);
	}

	return ReturnData;
}

template RCORELIBRARY_API FString	AssetManagerUtils::GetAssetTagValue<FString>(UAssetManager*, const FPrimaryAssetId&, FName);
template RCORELIBRARY_API FText		AssetManagerUtils::GetAssetTagValue<FText>(UAssetManager*, const FPrimaryAssetId&, FName);
template RCORELIBRARY_API int32		AssetManagerUtils::GetAssetTagValue<int32>(UAssetManager*, const FPrimaryAssetId&, FName);
template RCORELIBRARY_API float		AssetManagerUtils::GetAssetTagValue<float>(UAssetManager*, const FPrimaryAssetId&, FName);
template RCORELIBRARY_API bool		AssetManagerUtils::GetAssetTagValue<bool>(UAssetManager*, const FPrimaryAssetId&, FName);

void AssetManagerUtils::LoadPrimaryAsset(UObject* Outer, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	LoadPrimaryAsset(Outer, AssetManager, AssetId, MoveTemp(OnLoaded));
}

void AssetManagerUtils::LoadPrimaryAsset(UObject* Outer, UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded)
{
	if (!IsValid(AssetManager) || !IsValid(Outer) || !AssetId.IsValid())
	{
		OnLoaded(false, nullptr);
		return;
	}

	UObject* LoadedAsset = AssetManager->GetPrimaryAssetObject(AssetId);
	if (IsValid(LoadedAsset))
	{
		OnLoaded(false, LoadedAsset);
	}
	else
	{
		AssetManager->LoadPrimaryAsset(
			AssetId,
			TArray<FName>(),
			FStreamableDelegate::CreateWeakLambda(Outer, [AssetId, AssetManager, Callback = MoveTemp(OnLoaded)]()
				{
					if (!IsValid(AssetManager))
					{
						Callback(false, nullptr);
						return;
					}
					UObject* NewlyLoadedAsset = AssetManager->GetPrimaryAssetObject(AssetId);
					Callback(true, NewlyLoadedAsset);
				}
			)
		);
	}
}

void AssetManagerUtils::LoadPrimaryAssets(UObject* Outer, UAssetManager* AssetManager, TArray<FPrimaryAssetId> AssetIds, TFunction<void(bool)> OnLoaded)
{
	if (!IsValid(AssetManager) || !IsValid(Outer))
	{
		OnLoaded(false);
		return;
	}

	AssetManager->LoadPrimaryAssets(AssetIds, TArray<FName>(), FStreamableDelegate::CreateWeakLambda(Outer, [Callback = MoveTemp(OnLoaded)]() { Callback(true); }));
}


// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "AssetManagerUtils.h"

// Engine Headers
#include "Engine/AssetManager.h"

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

void AssetManagerUtils::LoadPrimaryAsset(UObject* Outer, UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, TFunction<void(FPrimaryAssetId, UObject*)> OnLoaded)
{
	if (!IsValid(AssetManager) || !IsValid(Outer) || !AssetId.IsValid())
	{
		OnLoaded(AssetId, nullptr);
		return;
	}

	UObject* LoadedAsset = AssetManager->GetPrimaryAssetObject(AssetId);
	if (IsValid(LoadedAsset))
	{
		OnLoaded(AssetId, LoadedAsset);
	}
	else
	{
		AssetManager->LoadPrimaryAsset(
			AssetId,
			TArray<FName>(),
			FStreamableDelegate::CreateWeakLambda(Outer, [AssetManager, AssetId, Callback = MoveTemp(OnLoaded)]()
				{
					if (!IsValid(AssetManager))
					{
						Callback(AssetId, nullptr);
						return;
					}
					UObject* NewlyLoadedAsset = AssetManager->GetPrimaryAssetObject(AssetId);
					Callback(AssetId, NewlyLoadedAsset);
				}
			)
		);
	}
}


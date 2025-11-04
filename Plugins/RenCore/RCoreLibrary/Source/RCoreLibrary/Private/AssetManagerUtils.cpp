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


bool AssetManagerUtils::IsLoading(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId)
{
	if (!IsValid(AssetManager))
	{
		return false;
	}

	TSharedPtr<FStreamableHandle> Handle = AssetManager->GetPrimaryAssetHandle(AssetId);
	FStreamableHandle* StreamableHandle = Handle.Get();
	if (!StreamableHandle)
	{
		return false;
	}

	return StreamableHandle->IsLoadingInProgress();
}

bool AssetManagerUtils::WasCancelled(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId)
{
	if (!IsValid(AssetManager))
	{
		return false;
	}

	TSharedPtr<FStreamableHandle> Handle = AssetManager->GetPrimaryAssetHandle(AssetId);
	FStreamableHandle* StreamableHandle = Handle.Get();
	if (!StreamableHandle)
	{
		return false;
	}

	return StreamableHandle->WasCanceled();
}


bool AssetManagerUtils::CancelAnyPending(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId)
{
	if(!IsValid(AssetManager))
	{
		return false;
	}

	TSharedPtr<FStreamableHandle> Handle = AssetManager->GetPrimaryAssetHandle(AssetId);
	FStreamableHandle* StreamableHandle = Handle.Get();
	if (!StreamableHandle)
	{
		return false;
	}

	StreamableHandle->CancelHandle();
	return true;
}

void AssetManagerUtils::LoadPrimaryAsset(UObject* Outer, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded)
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	LoadPrimaryAsset(Outer, AssetManager, AssetId, MoveTemp(OnLoaded));
}

TSharedPtr<FStreamableHandle> AssetManagerUtils::LoadPrimaryAsset(UObject* Outer, UAssetManager* AssetManager, const FPrimaryAssetId& AssetId, TFunction<void(bool, UObject*)> OnLoaded)
{
	if (!IsValid(AssetManager) || !IsValid(Outer) || !AssetId.IsValid())
	{
		OnLoaded(false, nullptr);
		return nullptr;
	}

	UObject* Object = AssetManager->GetPrimaryAssetObject(AssetId);
	if (IsValid(Object))
	{
		OnLoaded(false, Object);
		return nullptr;
	}

	TWeakObjectPtr<UAssetManager> WeakManager(AssetManager);
	return AssetManager->LoadPrimaryAsset(
		AssetId,
		TArray<FName>(),
		FStreamableDelegate::CreateWeakLambda(Outer, [WeakManager, AssetId, Callback = MoveTemp(OnLoaded)]()
			{
				UAssetManager* Manager = WeakManager.Get();
				if (!IsValid(Manager))
				{
					Callback(false, nullptr);
					return;
				}
				UObject* Object = Manager->GetPrimaryAssetObject(AssetId);
				Callback(true, Object);
			}
		)
	);
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


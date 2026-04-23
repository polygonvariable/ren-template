// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Parent Header
#include "Manager/RAssetManager.h"

// Project Headers
#include "Definition/LatentHandle.h"


template<typename T>
TFuture<FLatentLoadedAsset<T>> URAssetManager::FetchPrimaryAsset(const FGuid& LatentId, const FPrimaryAssetId& AssetId, const TArray<FName>& AssetBundles)
{
	TSharedPtr<TPromise<FLatentLoadedAsset<T>>> Promise = MakeShared<TPromise<FLatentLoadedAsset<T>>>();
	TFuture<FLatentLoadedAsset<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, LatentId, AssetId, WeakHandle, Promise]()
		{
			FLatentLoadedAsset<T> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				T* Object = Cast<T>(GetPrimaryAssetObject(AssetId));

				Result.Asset = Object;
				Result.bSuccess = (Object != nullptr);
				Result.bCancelled = false;
			}

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);
	
	LoadPrimaryAsset(AssetId, AssetBundles, MoveTemp(Streamable));
	return Future;
}

template<typename T>
TFuture<FLatentLoadedAssets<T>> URAssetManager::FetchPrimaryAssets(const FGuid& LatentId, const TArray<FPrimaryAssetId>& AssetIds, const TArray<FName>& AssetBundles, bool bResolveObjects)
{
	TSharedPtr<TPromise<FLatentLoadedAssets<T>>> Promise = MakeShared<TPromise<FLatentLoadedAssets<T>>>();
	TFuture<FLatentLoadedAssets<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, LatentId, AssetIds, bResolveObjects, WeakHandle, Promise]()
		{
			FLatentLoadedAssets<T> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				if (bResolveObjects)
				{
					TArray<T*> Objects;
					for (const FPrimaryAssetId& AssetId : AssetIds)
					{
						T* Object = Cast<T>(GetPrimaryAssetObject(AssetId));
						if (Object)
						{
							Objects.Add(Object);
						}
					}

					Result.bSuccess = (Objects.Num() == AssetIds.Num());
					Result.bCancelled = false;
					Result.Assets = MoveTemp(Objects);
				}
				else
				{
					Result.bSuccess = true;
					Result.bCancelled = false;
				}
			}

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);

	LoadPrimaryAssets(AssetIds, AssetBundles, MoveTemp(Streamable));
	return Future;
}


template<typename T>
TFuture<FLatentLoadedAsset<T>> URAssetManager::FetchSecondaryAsset(const FGuid& LatentId, const FSoftObjectPath& Path)
{
	TSharedPtr<TPromise<FLatentLoadedAsset<T>>> Promise = MakeShared<TPromise<FLatentLoadedAsset<T>>>();
	TFuture<FLatentLoadedAsset<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, LatentId, WeakHandle, Promise]()
		{
			FLatentLoadedAsset<T> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				T* Object = Cast<T>(Path.ResolveObject());

				Result.Asset = Object;
				Result.bSuccess = (Object != nullptr);
				Result.bCancelled = false;
			}

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Path, MoveTemp(Streamable));

	return Future;
}

template<typename T>
TFuture<FLatentLoadedAssets<T>> URAssetManager::FetchSecondaryAssets(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths)
{
	TSharedPtr<TPromise<FLatentLoadedAssets<T>>> Promise = MakeShared<TPromise<FLatentLoadedAssets<T>>>();
	TFuture<FLatentLoadedAssets<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, LatentId, WeakHandle, Promise]()
		{
			FLatentLoadedAssets<T> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				TArray<T*> Objects;
				for (const FSoftObjectPath& Path : Paths)
				{
					T* Object = Cast<T>(Path.ResolveObject());
					if (Object)
					{
						Objects.Add(Object);
					}
				}

				Result.bSuccess = (Objects.Num() == Paths.Num());
				Result.bCancelled = false;
				Result.Assets = MoveTemp(Objects);
			}

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Paths, MoveTemp(Streamable));

	return Future;
}


template<typename T>
TFuture<FLatentLoadedAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path)
{
	return FetchSecondaryClass(LatentId, Path, T::StaticClass());
}

template<typename T>
TFuture<FLatentLoadedAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths)
{
	return FetchSecondaryClasses(LatentId, Paths, T::StaticClass());
}


template<typename T>
TFuture<FLatentLoadedAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const TSoftClassPtr<T>& Path)
{
	return FetchSecondaryClass(LatentId, Path.ToSoftObjectPath(), T::StaticClass());
}

template<typename T>
TFuture<FLatentLoadedAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<TSoftClassPtr<T>>& Paths)
{
	TArray<FSoftObjectPath> ObjectPaths;
	for (const TSoftClassPtr<T>& Path : Paths)
	{
		ObjectPaths.Add(Path.ToSoftObjectPath());
	}
	return FetchSecondaryClasses(LatentId, ObjectPaths, T::StaticClass());
}


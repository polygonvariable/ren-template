// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Parent Header
#include "RAssetManager.h"

// Project Headers
#include "RCoreAssetManager/Public/LatentHandle.h"



template<typename T>
TFuture<FLatentResultAsset<T>> URAssetManager::FetchPrimaryAsset(const FPrimaryAssetId& AssetId)
{
	TSharedPtr<TPromise<FLatentResultAsset<T>>> Promise = MakeShared<TPromise<FLatentResultAsset<T>>>();
	TFuture<FLatentResultAsset<T>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, AssetId, Promise]()
		{
			T* Object = Cast<T>(GetPrimaryAssetObject(AssetId));

			FLatentResultAsset<T> Result;
			Result.Asset = Object;
			Result.bSuccess = (Object != nullptr);

			Promise->SetValue(MoveTemp(Result));
		}
	);

	LoadPrimaryAsset(AssetId, TArray<FName>(), MoveTemp(Streamable));
	return Future;
}

template<typename T>
TFuture<FLatentResultAssets<T>> URAssetManager::FetchPrimaryAssets(const TArray<FPrimaryAssetId>& AssetIds)
{
	TSharedPtr<TPromise<FLatentResultAssets<T>>> Promise = MakeShared<TPromise<FLatentResultAssets<T>>>();
	TFuture<FLatentResultAssets<T>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, AssetIds, Promise]()
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

			FLatentResultAssets<T> Result;
			Result.bSuccess = (Objects.Num() == AssetIds.Num());
			Result.Assets = MoveTemp(Objects);

			Promise->SetValue(MoveTemp(Result));
		}
	);

	LoadPrimaryAssets(AssetIds, TArray<FName>(), MoveTemp(Streamable));
	return Future;
}

template<typename T>
TFuture<FLatentResultAsset<T>> URAssetManager::FetchPrimaryAsset(const FGuid& LatentId, const FPrimaryAssetId& AssetId)
{
	TSharedPtr<TPromise<FLatentResultAsset<T>>> Promise = MakeShared<TPromise<FLatentResultAsset<T>>>();
	TFuture<FLatentResultAsset<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, AssetId, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAsset<T> Result;
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

	LoadPrimaryAsset(AssetId, TArray<FName>(), MoveTemp(Streamable));
	return Future;
}

template<typename T>
TFuture<FLatentResultAssets<T>> URAssetManager::FetchPrimaryAssets(const FGuid& LatentId, const TArray<FPrimaryAssetId>& AssetIds)
{
	TSharedPtr<TPromise<FLatentResultAssets<T>>> Promise = MakeShared<TPromise<FLatentResultAssets<T>>>();
	TFuture<FLatentResultAssets<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, AssetIds, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAssets<T> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
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

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);

	LoadPrimaryAssets(AssetIds, TArray<FName>(), MoveTemp(Streamable));
	return Future;
}


template<typename T>
TFuture<FLatentResultAsset<T>> URAssetManager::FetchSecondaryAsset(const FSoftObjectPath& Path)
{
	TSharedPtr<TPromise<FLatentResultAsset<T>>> Promise = MakeShared<TPromise<FLatentResultAsset<T>>>();
	TFuture<FLatentResultAsset<T>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, Promise]()
		{
			FLatentResultAsset<T> Result;

			T* Object = Cast<T>(Path.ResolveObject());

			Result.Asset = Object;
			Result.bSuccess = (Object != nullptr);

			Promise->SetValue(MoveTemp(Result));
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Path, MoveTemp(Streamable));

	return Future;
}

template<typename T>
TFuture<FLatentResultAssets<T>> URAssetManager::FetchSecondaryAssets(const TArray<FSoftObjectPath>& Paths)
{
	TSharedPtr<TPromise<FLatentResultAssets<T>>> Promise = MakeShared<TPromise<FLatentResultAssets<T>>>();
	TFuture<FLatentResultAssets<T>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, Promise]()
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

			FLatentResultAssets<T> Result;
			Result.bSuccess = (Objects.Num() == Paths.Num());
			Result.Assets = MoveTemp(Objects);

			Promise->SetValue(MoveTemp(Result));
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Paths, MoveTemp(Streamable));

	return Future;
}

template<typename T>
TFuture<FLatentResultAsset<T>> URAssetManager::FetchSecondaryAsset(const FGuid& LatentId, const FSoftObjectPath& Path)
{
	TSharedPtr<TPromise<FLatentResultAsset<T>>> Promise = MakeShared<TPromise<FLatentResultAsset<T>>>();
	TFuture<FLatentResultAsset<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAsset<T> Result;
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
TFuture<FLatentResultAssets<T>> URAssetManager::FetchSecondaryAssets(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths)
{
	TSharedPtr<TPromise<FLatentResultAssets<T>>> Promise = MakeShared<TPromise<FLatentResultAssets<T>>>();
	TFuture<FLatentResultAssets<T>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAssets<T> Result;
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
TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FSoftClassPath& Path)
{
	return FetchSecondaryClass(Path, T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths)
{
	return FetchSecondaryClasses(Paths, T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path)
{
	return FetchSecondaryClass(LatentId, Path, T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths)
{
	return FetchSecondaryClasses(LatentId, Paths, T::StaticClass());
}


template<typename T>
TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const TSoftClassPtr<T>& Path)
{
	return FetchSecondaryClass(Path.ToSoftObjectPath(), T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const TArray<TSoftClassPtr<T>>& Paths)
{
	TArray<FSoftObjectPath> ObjectPaths;
	for (const TSoftClassPtr<T>& Path : Paths)
	{
		ObjectPaths.Add(Path.ToSoftObjectPath());
	}
	return FetchSecondaryClasses(ObjectPaths, T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const TSoftClassPtr<T>& Path)
{
	return FetchSecondaryClass(LatentId, Path.ToSoftObjectPath(), T::StaticClass());
}

template<typename T>
TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<TSoftClassPtr<T>>& Paths)
{
	TArray<FSoftObjectPath> ObjectPaths;
	for (const TSoftClassPtr<T>& Path : Paths)
	{
		ObjectPaths.Add(Path.ToSoftObjectPath());
	}
	return FetchSecondaryClasses(LatentId, ObjectPaths, T::StaticClass());
}


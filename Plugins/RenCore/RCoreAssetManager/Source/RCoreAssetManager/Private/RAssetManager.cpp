// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "RAssetManager.h"
#include "RAssetManager.inl"

// Engine Headers



TWeakPtr<FLatentHandle> URAssetManager::CreateHandle(const FGuid& LatentId)
{
	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	TWeakPtr<FLatentHandle> WeakHandle(Handle);

	LatentHandles.Add(LatentId, Handle);

	return WeakHandle;
}

void URAssetManager::RemoveHandle(const FGuid& LatentId)
{
	LatentHandles.Remove(LatentId);
}

void URAssetManager::CancelFetch(const FGuid& LatentId)
{
	FScopeLock Lock(&LatentHandleLock);
	TSharedPtr<FLatentHandle>* Handle = LatentHandles.Find(LatentId);
	if (Handle)
	{
		FLatentHandle* LatentHandle = Handle->Get();
		if (LatentHandle)
		{
			UE_LOG(LogTemp, Warning, TEXT("Item loading cancelled"));
			LatentHandle->Cancel();
		}
		// LatentHandles.Remove(LatentId);
	}
}


TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FSoftClassPath& Path, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAsset<UClass>>> Promise = MakeShared<TPromise<FLatentResultAsset<UClass>>>();
	TFuture<FLatentResultAsset<UClass>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, Type, Promise]()
		{
			UClass* Class = Path.ResolveClass();

			FLatentResultAsset<UClass> Result;
			Result.Asset = Class;
			Result.bSuccess = (Class != nullptr && Class->IsChildOf(Type));

			Promise->SetValue(MoveTemp(Result));
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Path, MoveTemp(Streamable));

	return Future;
}

TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAssets<UClass>>> Promise = MakeShared<TPromise<FLatentResultAssets<UClass>>>();
	TFuture<FLatentResultAssets<UClass>> Future = Promise->GetFuture();

	if (Paths.Num() == 0)
	{
		FLatentResultAssets<UClass> Result;
		Result.bSuccess = false;
		Result.bCancelled = false;

		Promise->SetValue(MoveTemp(Result));

		return Future;
	}

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, Type, Promise]()
		{
			TArray<UClass*> Classes;
			for (const FSoftClassPath& Path : Paths)
			{
				UClass* Class = Path.ResolveClass();
				if (Class && Class->IsChildOf(Type))
				{
					Classes.Add(Class);
				}
			}

			FLatentResultAssets<UClass> Result;
			Result.bSuccess = (Classes.Num() == Paths.Num());
			Result.Assets = MoveTemp(Classes);

			Promise->SetValue(MoveTemp(Result));
		}
	);

	TArray<FSoftObjectPath> ObjectPaths;
	for (const FSoftClassPath& Path : Paths)
	{
		ObjectPaths.Add(Path);
	}

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(ObjectPaths, MoveTemp(Streamable));

	return Future;
}

TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAsset<UClass>>> Promise = MakeShared<TPromise<FLatentResultAsset<UClass>>>();
	TFuture<FLatentResultAsset<UClass>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, Type, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAsset<UClass> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				UClass* Class = Path.ResolveClass();

				Result.Asset = Class;
				Result.bSuccess = (Class != nullptr && Class->IsChildOf(Type));
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

TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAssets<UClass>>> Promise = MakeShared<TPromise<FLatentResultAssets<UClass>>>();
	TFuture<FLatentResultAssets<UClass>> Future = Promise->GetFuture();

	if (Paths.Num() == 0)
	{
		FLatentResultAssets<UClass> Result;
		Result.bSuccess = false;
		Result.bCancelled = false;

		Promise->SetValue(MoveTemp(Result));

		return Future;
	}

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, Type, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAssets<UClass> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				TArray<UClass*> Classes;
				for (const FSoftClassPath& Path : Paths)
				{
					UClass* Class = Path.ResolveClass();
					if (Class && Class->IsChildOf(Type))
					{
						Classes.Add(Class);
					}
				}

				Result.bSuccess = (Classes.Num() == Paths.Num());
				Result.bCancelled = false;
				Result.Assets = MoveTemp(Classes);
			}

			Promise->SetValue(MoveTemp(Result));

			FScopeLock Lock(&LatentHandleLock);
			LatentHandles.Remove(LatentId);
		}
	);

	TArray<FSoftObjectPath> ObjectPaths;
	for (const FSoftClassPath& Path : Paths)
	{
		ObjectPaths.Add(Path);
	}

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(ObjectPaths, MoveTemp(Streamable));

	return Future;
}


TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FSoftObjectPath& Path, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAsset<UClass>>> Promise = MakeShared<TPromise<FLatentResultAsset<UClass>>>();
	TFuture<FLatentResultAsset<UClass>> Future = Promise->GetFuture();

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, Type, Promise]()
		{
			UClass* Class = Cast<UClass>(Path.ResolveObject());

			FLatentResultAsset<UClass> Result;
			Result.Asset = Class;
			Result.bSuccess = (Class != nullptr && Class->IsChildOf(Type));

			Promise->SetValue(MoveTemp(Result));
		}
	);

	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Path, MoveTemp(Streamable));

	return Future;
}

TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const TArray<FSoftObjectPath>& Paths, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAssets<UClass>>> Promise = MakeShared<TPromise<FLatentResultAssets<UClass>>>();
	TFuture<FLatentResultAssets<UClass>> Future = Promise->GetFuture();

	if (Paths.Num() == 0)
	{
		FLatentResultAssets<UClass> Result;
		Result.bSuccess = false;
		Result.bCancelled = false;

		Promise->SetValue(MoveTemp(Result));

		return Future;
	}

	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, Type, Promise]()
		{
			TArray<UClass*> Classes;
			for (const FSoftObjectPath& Path : Paths)
			{
				UClass* Class = Cast<UClass>(Path.ResolveObject());
				if (Class && Class->IsChildOf(Type))
				{
					Classes.Add(Class);
				}
			}

			FLatentResultAssets<UClass> Result;
			Result.bSuccess = (Classes.Num() == Paths.Num());
			Result.Assets = MoveTemp(Classes);

			Promise->SetValue(MoveTemp(Result));
		}
	);


	FStreamableManager& Manager = GetStreamableManager();
	Manager.RequestAsyncLoad(Paths, MoveTemp(Streamable));

	return Future;
}

TFuture<FLatentResultAsset<UClass>> URAssetManager::FetchSecondaryClass(const FGuid& LatentId, const FSoftObjectPath& Path, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAsset<UClass>>> Promise = MakeShared<TPromise<FLatentResultAsset<UClass>>>();
	TFuture<FLatentResultAsset<UClass>> Future = Promise->GetFuture();

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Path, Type, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAsset<UClass> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				UClass* Class = Cast<UClass>(Path.ResolveObject());

				Result.Asset = Class;
				Result.bSuccess = (Class != nullptr && Class->IsChildOf(Type));
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

TFuture<FLatentResultAssets<UClass>> URAssetManager::FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths, UClass* Type)
{
	TSharedPtr<TPromise<FLatentResultAssets<UClass>>> Promise = MakeShared<TPromise<FLatentResultAssets<UClass>>>();
	TFuture<FLatentResultAssets<UClass>> Future = Promise->GetFuture();

	if (Paths.Num() == 0)
	{
		FLatentResultAssets<UClass> Result;
		Result.bSuccess = false;
		Result.bCancelled = false;

		Promise->SetValue(MoveTemp(Result));

		return Future;
	}

	TSharedPtr<FLatentHandle> Handle = MakeShared<FLatentHandle, ESPMode::ThreadSafe>();
	{
		FScopeLock Lock(&LatentHandleLock);
		LatentHandles.Add(LatentId, Handle);
	};

	TWeakPtr<FLatentHandle> WeakHandle(Handle);
	FStreamableDelegate Streamable = FStreamableDelegate::CreateWeakLambda(this, [this, Paths, Type, LatentId, WeakHandle, Promise]()
		{
			FLatentResultAssets<UClass> Result;
			TSharedPtr<FLatentHandle> Handle = WeakHandle.Pin();

			if (Handle.IsValid() && Handle->IsCancelled())
			{
				Result.bSuccess = false;
				Result.bCancelled = true;
			}
			else
			{
				TArray<UClass*> Classes;
				for (const FSoftObjectPath& Path : Paths)
				{
					UClass* Class = Cast<UClass>(Path.ResolveObject());
					if (Class && Class->IsChildOf(Type))
					{
						Classes.Add(Class);
					}
				}

				Result.bSuccess = (Classes.Num() == Paths.Num());
				Result.bCancelled = false;
				Result.Assets = MoveTemp(Classes);
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


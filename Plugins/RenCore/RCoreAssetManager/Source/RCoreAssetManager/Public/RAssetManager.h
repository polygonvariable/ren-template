// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "RCoreAssetManager/Public/LatentResult.h"

// Generated Headers
#include "RAssetManager.generated.h"

// Forward Declarations
struct FLatentHandle;



/**
 *
 */
UCLASS(MinimalAPI, Config = Game)
class URAssetManager : public UAssetManager
{

	GENERATED_BODY()

public:

	RCOREASSETMANAGER_API void CancelFetch(const FGuid& LatentId);

	RCOREASSETMANAGER_API TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FSoftClassPath& Path, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths, UClass* Type);

	RCOREASSETMANAGER_API TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FSoftObjectPath& Path, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftObjectPath>& Paths, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftObjectPath& Path, UClass* Type);
	RCOREASSETMANAGER_API TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths, UClass* Type);


	template<typename T>
	TFuture<FLatentResultAsset<T>> FetchPrimaryAsset(const FPrimaryAssetId& AssetId);

	template<typename T>
	TFuture<FLatentResultAssets<T>> FetchPrimaryAssets(const TArray<FPrimaryAssetId>& AssetIds);

	template<typename T>
	TFuture<FLatentResultAsset<T>> FetchPrimaryAsset(const FGuid& LatentId, const FPrimaryAssetId& AssetId);

	template<typename T>
	TFuture<FLatentResultAssets<T>> FetchPrimaryAssets(const FGuid& LatentId, const TArray<FPrimaryAssetId>& AssetIds);


	template<typename T>
	TFuture<FLatentResultAsset<T>> FetchSecondaryAsset(const FSoftObjectPath& Path);

	template<typename T>
	TFuture<FLatentResultAssets<T>> FetchSecondaryAssets(const TArray<FSoftObjectPath>& Paths);

	template<typename T>
	TFuture<FLatentResultAsset<T>> FetchSecondaryAsset(const FGuid& LatentId, const FSoftObjectPath& Path);

	template<typename T>
	TFuture<FLatentResultAssets<T>> FetchSecondaryAssets(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths);
	

	template<typename T>
	TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FSoftClassPath& Path);

	template<typename T>
	TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const TArray<FSoftClassPath>& Paths);

	template<typename T>
	TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path);

	template<typename T>
	TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths);


	template<typename T>
	TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const TSoftClassPtr<T>& Path);

	template<typename T>
	TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const TArray<TSoftClassPtr<T>>& Paths);

	template<typename T>
	TFuture<FLatentResultAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const TSoftClassPtr<T>& Path);

	template<typename T>
	TFuture<FLatentResultAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<TSoftClassPtr<T>>& Paths);

protected:

	TMap<FGuid, TSharedPtr<FLatentHandle>> LatentHandles;
	FCriticalSection LatentHandleLock;

	TWeakPtr<FLatentHandle> CreateHandle(const FGuid& LatentId);
	void RemoveHandle(const FGuid& LatentId);

};


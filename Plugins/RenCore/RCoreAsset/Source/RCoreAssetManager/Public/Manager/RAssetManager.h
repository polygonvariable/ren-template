// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/AssetManager.h"

// Project Headers
#include "Definition/LatentResult.h"

// Generated Headers
#include "RAssetManager.generated.h"

// Module Macros
#define REN_API RCOREASSETMANAGER_API

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

	UE_DEPRECATED(5.4, "Use FStreamableDelegate to cancel loading.")
	REN_API void CancelFetch(const FGuid& LatentId);

	REN_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path, UClass* Type);
	REN_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths, UClass* Type);

	REN_API TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftObjectPath& Path, UClass* Type);
	REN_API TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths, UClass* Type);

	template<typename T>
	UE_DEPRECATED(5.4, "Use the LoadPrimaryAsset function.")
	TFuture<FLatentLoadedAsset<T>> FetchPrimaryAsset(const FGuid& LatentId, const FPrimaryAssetId& AssetId, const TArray<FName>& AssetBundles = TArray<FName>());

	template<typename T>
	UE_DEPRECATED(5.4, "Use the LoadPrimaryAssets function.")
	TFuture<FLatentLoadedAssets<T>> FetchPrimaryAssets(const FGuid& LatentId, const TArray<FPrimaryAssetId>& AssetIds, const TArray<FName>& AssetBundles = TArray<FName>(), bool bResolveObjects = true);


	template<typename T>
	TFuture<FLatentLoadedAsset<T>> FetchSecondaryAsset(const FGuid& LatentId, const FSoftObjectPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<T>> FetchSecondaryAssets(const FGuid& LatentId, const TArray<FSoftObjectPath>& Paths);
	

	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const FSoftClassPath& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<FSoftClassPath>& Paths);


	template<typename T>
	TFuture<FLatentLoadedAsset<UClass>> FetchSecondaryClass(const FGuid& LatentId, const TSoftClassPtr<T>& Path);

	template<typename T>
	TFuture<FLatentLoadedAssets<UClass>> FetchSecondaryClasses(const FGuid& LatentId, const TArray<TSoftClassPtr<T>>& Paths);

protected:

	TSet<FPrimaryAssetId> LoadingAssetsIds;

	TMap<FGuid, TSharedPtr<FLatentHandle>> LatentHandles;
	FCriticalSection LatentHandleLock;

	TWeakPtr<FLatentHandle> CreateHandle(const FGuid& LatentId);
	void RemoveHandle(const FGuid& LatentId);

public:

	static REN_API URAssetManager* Get()
	{
		return Cast<URAssetManager>(UAssetManager::GetIfInitialized());
	}

};


// Module Macros
#undef REN_API


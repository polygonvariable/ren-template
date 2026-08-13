// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCOREASSETINSTANCE_API

// Forward Declarations
class IAssetInstanceCollectionProvider;
class IAssetInstanceCollection;


/**
 *
 */
class FAssetInstanceUtil
{

public:

	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetId& AssetId);
	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetId& AssetId);

	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetType& AssetType);
	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetType& AssetType);


	REN_API static IAssetInstanceCollection* GetInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId);
	REN_API static IAssetInstanceCollection* GetInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId);

	REN_API static IAssetInstanceCollection* GetPrimaryInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType);
	REN_API static IAssetInstanceCollection* GetPrimaryInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType);

};


// Module Macros
#undef REN_API


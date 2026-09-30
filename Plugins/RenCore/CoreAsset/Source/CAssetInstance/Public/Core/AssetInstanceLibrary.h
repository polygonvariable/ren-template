// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Forward Declarations
class IAssetInstanceCollectionProvider;
class IAssetInstanceCollection;


/**
 *
 */
class FAssetInstanceLibrary
{

public:

	CASSETINSTANCE_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetId& AssetId);
	CASSETINSTANCE_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetId& AssetId);

	CASSETINSTANCE_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetType& AssetType);
	CASSETINSTANCE_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetType& AssetType);


	CASSETINSTANCE_API static IAssetInstanceCollection* GetInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId);
	CASSETINSTANCE_API static IAssetInstanceCollection* GetInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType, const FName& CollectionId);

	CASSETINSTANCE_API static IAssetInstanceCollection* GetPrimaryInstanceCollection(UWorld* Context, const FPrimaryAssetType& AssetType);
	CASSETINSTANCE_API static IAssetInstanceCollection* GetPrimaryInstanceCollection(UGameInstance* Context, const FPrimaryAssetType& AssetType);

};


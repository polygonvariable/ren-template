// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Module Macros
#define REN_API RCOREASSETINSTANCE_API

// Forward Declarations
class IAssetInstanceCollectionProvider;


class FAssetInstanceUtil
{

public:

	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetId& AssetId);
	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetId& AssetId);

	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UWorld* Context, const FPrimaryAssetType& AssetType);
	REN_API static IAssetInstanceCollectionProvider* GetInstanceCollectionProvider(UGameInstance* Context, const FPrimaryAssetType& AssetType);

};


// Module Macros
#undef REN_API


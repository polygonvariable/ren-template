// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/TaskType.h"

// Generated Headers
#include "ShopSubsystem.generated.h"

// Module Macros
#define REN_API RSHOP_API

// Forward Declarations
class UAssetCollection;
class UShopStorageManager;
class UTradeAsset;
class UCoreDataAsset;
class IStorageProvider;
struct FTradeAssetDetail;
struct FInstancedStruct;


/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UShopSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UShopStorageManager* GetStorageManager();

	REN_API bool TryPurchaseItem(const FPrimaryAssetId& ShopAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId);

	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const;
	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const;

	REN_API void QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&)> Callback);

protected:

	IStorageProvider* StorageProvider;


	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	REN_API static UShopSubsystem* Get(UWorld* World);
	REN_API static UShopSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API


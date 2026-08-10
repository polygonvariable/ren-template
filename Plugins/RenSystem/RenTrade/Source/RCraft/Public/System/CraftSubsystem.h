// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Core/Type/CraftQuery.h"

// Generated Headers
#include "CraftSubsystem.generated.h"

// Module Macros
#define REN_API RCRAFT_API

// Forward Declarations
class UAssetCollection;
class IStorageProvider;
class UCraftStorageManager;
class UTradeAsset;
class UCoreDataAsset;

struct FTradeAssetDetail;
struct FInstancedStruct;
struct FCraftInstance;


/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UCraftSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UCraftStorageManager* GetStorageManager();

	REN_API bool TryClaimCraftItem(const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId);
	REN_API bool TryCraftItem(const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId);

	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const;
	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const;

	REN_API void QueryItems(const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)> Callback);

protected:

	IStorageProvider* StorageProvider;


	void QueryAssetItems(const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)>&& Callback);
	void QueryStorageItems(const TMap<UCoreDataAsset*, FTradeAssetDetail>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorageManager* StorageManager, TFunctionRef<void(const FPrimaryAssetId&, const FTradeAssetDetail&, const FCraftInstance*)>&& Callback);

	void OnPreGameInitialized();

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	REN_API static UCraftSubsystem* Get(UWorld* World);
	REN_API static UCraftSubsystem* Get(UGameInstance* GameInstance);

};


// Module Macros
#undef REN_API


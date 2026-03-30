// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "Definition/CraftQuery.h"
#include "Definition/TaskType.h"

// Generated Headers
#include "CraftSubsystem.generated.h"

// Module Macros
#define REN_API RCRAFT_API

// Forward Declarations
class UAssetCollection;
class UStorage;
class IStorageProvider;
class UCraftStorage;
class UTradeAsset;
class UCoreDataAsset;

struct FAssetDetail_Trade;
struct FInstancedStruct;
struct FCraftData;



/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintType)
class UCraftSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	REN_API UCraftStorage* GetCraft(const FName& CraftId);

	REN_API void ClaimCraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);
	REN_API void CraftItem(const FGuid& TaskId, const FPrimaryAssetId& CraftAssetId, const FGuid& TradeCollectionId, const FPrimaryAssetId& TargetAssetId, FTaskCallback Callback);

	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FInstancedStruct& Context) const;
	REN_API const UAssetCollection* GetMaterialCollection(const UCoreDataAsset* Asset, const FGuid& CollectionId) const;

	REN_API void QueryItems(const FName& CraftId, const UTradeAsset* Asset, const FGuid& CollectionId, ECraftQuerySource QuerySource, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)> Callback);

protected:

	TWeakInterfacePtr<IStorageProvider> StorageProvider;


	void QueryAssetItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback);
	void HandleStorageItems(const TMap<UCoreDataAsset*, FAssetDetail_Trade>& AssetList, const FPrimaryAssetId& CraftAssetId, const FGuid& CollectionId, const FInstancedStruct& Context, UCraftStorage* CraftStorage, TFunctionRef<void(const FPrimaryAssetId&, const FAssetDetail_Trade&, const FCraftData*)>&& Callback);

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


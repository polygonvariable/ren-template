// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/Runtime/AvatarInstance.h"
#include "Interface/AscensionInstanceProvider.h"
#include "Interface/AssetInstanceCollection.h"
#include "SaveGame/Storage.h"

// Generated Headers
#include "AvatarStorage.generated.h"

// Module Macros
#define REN_API RAVATAR_API

// Forward Declarations
class UAssetManager;
class UFilterCriterion;
struct FAvatarQueryRule;
struct FAvatarSortEntry;


/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarStorage : public UStorage, public IAssetInstanceCollection, public IAscensionInstanceProvider
{

	GENERATED_BODY()

public:

	// ~ UStorage
	virtual void InitializeDefaults() override;
	// ~ End of UStorage

	// ~ IAssetInstanceCollection
	virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool AddInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool RemoveInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) override;

	virtual bool RemoveAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) override;
	virtual bool RemoveInstanceById(const FPrimaryAssetId& AssetId, const FGuid& InstanceId, int Quantity) override;

	virtual bool ContainInstances(const TMap<FPrimaryAssetId, int>& AssetIds, int Multiplier) const override;
	virtual bool ContainAnyInstances(const TMap<FPrimaryAssetId, int>& InAssetIds, int InMultiplier, FPrimaryAssetId& OutAssetId, int& OutQuantity) const override;

	virtual FGameEventDelegate& GetOnAssetInstanceCollectionUpdated() override;
	// ~ End of IAssetInstanceCollection

	// ~ IAscensionInstanceProvider
	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const override;
	// ~ End of IAscensionInstanceProvider

	REN_API const FAvatarInstance* GetInstance(const FPrimaryAssetId& AssetId) const;
	REN_API const FAvatarInstance* GetInstanceById(const FGuid& InstanceId) const;

	REN_API bool UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FAvatarInstance*)> Callback);
	REN_API void QueryInstances(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TFunctionRef<void(const FAvatarSortEntry&)> Callback);

protected:

	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, FAvatarInstance> AvatarCollection;


	void HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;

	bool AddInstance_Internal(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId);
	bool RemoveInstance_Internal(const FPrimaryAssetId& AssetId);

};


// Module Macros
#undef REN_API


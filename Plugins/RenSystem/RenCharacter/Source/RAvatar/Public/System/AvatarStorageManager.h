// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"
#include "Core/Interface/AscensionInstanceProvider.h"
#include "Core/Interface/AssetInstanceCollection.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "AvatarStorageManager.generated.h"

// Module Macros
#define REN_API RAVATAR_API

// Forward Declarations
class UAvatarStorage;
class UAssetManager;
class UFilterCriterion;
struct FAvatarInstance;
struct FAvatarQueryRule;
struct FAvatarSortEntry;


/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarStorageManager : public UObject, public IStorageManager, public IAssetInstanceCollection, public IAscensionInstanceProvider
{

	GENERATED_BODY()

public:

	FGameEventDelegate OnStorageUpdated;


	REN_API const FAvatarInstance* GetInstance(const FPrimaryAssetId& AssetId) const;
	REN_API const FAvatarInstance* GetInstanceById(const FGuid& InstanceId) const;

	REN_API bool UpdateInstance(const FPrimaryAssetId& AssetId, TFunctionRef<void(FAvatarInstance*)> Callback);
	REN_API void QueryInstances(const UFilterCriterion* FilterCriterion, const FAvatarQueryRule& QueryRule, TArray<FAvatarSortEntry>& OutSortedItems);

	// ~ IAssetInstanceCollection
	virtual bool AddInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool RemoveInstance(const FPrimaryAssetId& AssetId, int Quantity) override;
	virtual bool ContainInstance(const FPrimaryAssetId& AssetId, int Quantity) const override;
	virtual FGameEventDelegate& GetOnAssetInstanceCollectionUpdated() override;
	// ~ End of IAssetInstanceCollection

	// ~ IAscensionInstanceProvider
	virtual const FAscensionData* GetAscensionInstance(const FPrimaryAssetId& AssetId, const FGuid& InstanceId) const override;
	// ~ End of IAscensionInstanceProvider
	
	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

protected:

	UPROPERTY()
	TObjectPtr<UAvatarStorage> LocalStorage = nullptr;


	void HandleItemSorting(TArray<FAvatarSortEntry>& SortedItems, const FAvatarQueryRule& QueryRule) const;
	void QueryAssetItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;
	void QueryInstanceItems(UAssetManager* AssetManager, const UFilterCriterion* FilterCriterion, TArray<FAvatarSortEntry>& OutSortedItems) const;

	bool AddInstance_Internal(UAssetManager* AssetManager, const FPrimaryAssetId& AssetId);
	bool RemoveInstance_Internal(const FPrimaryAssetId& AssetId);

};


// Module Macros
#undef REN_API


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "CraftItemSubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class UInventorySubsystem;
class UCounterSubsystem;

struct FExchangeRule;
struct FExchangeQuota;



/**
 *
 */
UCLASS()
class UCraftItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void CraftItem(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	void CraftItem(FName CraftId, FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, const FExchangeQuota& Quota);

	bool ResetCrafter(FName CraftId);

protected:

	UAssetManager* AssetManager;
	TWeakObjectPtr<UCounterSubsystem> CounterSubsystem;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	bool HandleItemCrafting(FName CraftId, FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, const FExchangeQuota& ExchangeQuantity, UObject* AssetObject);
	bool HandleItemCrafting(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject);

	void HandleGameInitialized();

public:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemCraft, bool /* bSuccess */);
	FOnItemCraft OnItemCrafted;

};


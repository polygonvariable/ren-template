// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "ExchangeItemSubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class ICounterManagerInterface;
class UInventorySubsystem;

struct FExchangeRule;
struct FExchangeQuota;



/**
 *
 */
UCLASS(Abstract)
class UExchangeItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void Exchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);
	void Exchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, FName CounterId, const FExchangeQuota& Quota);

protected:

	UAssetManager* AssetManager;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakInterfacePtr<ICounterManagerInterface> CounterManagerInterface;


	virtual const FExchangeRule* GetExchangeRule(UObject* Target) const;

	// ~ Bindings
	virtual bool HandleItemExchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject);
	virtual bool HandleItemExchange(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject, FName CounterId, const FExchangeQuota& Quota);
	
	virtual void HandleGameInitialized();
	// ~ End of Bindings

public:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemExchanged, bool /* bSuccess */);
	FOnItemExchanged OnItemExchanged;

};


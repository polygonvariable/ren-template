// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "PurchaseItemSubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class UInventorySubsystem;

struct FExchangeRule;



/**
 *
 */
UCLASS()
class UPurchaseItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void Purchase(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity);

protected:

	UAssetManager* AssetManager;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	bool HandlePurchase(FName ContainerId, const FPrimaryAssetId& AssetId, int Quantity, UObject* AssetObject);

	void HandleGameInitialized();

public:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPurchase, FName /* TargetId */, bool /* bSuccess */);
	FOnItemPurchase OnItemPurchase;

};


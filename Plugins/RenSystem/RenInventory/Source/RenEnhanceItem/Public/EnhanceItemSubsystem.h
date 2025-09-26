// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RCoreInventory/Public/InventoryRecord.h"

// Generated Headers
#include "EnhanceItemSubsystem.generated.h"

// Forward Declarations
class UEnhanceableAsset;
class UInventorySubsystem;

class UAssetManager;



/**
 *
 */
UCLASS()
class UEnhanceItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemEnhance, FName /* TargetId */, bool /* bSuccess */);

	FOnItemEnhance OnItemLevelUp;
	FOnItemEnhance OnItemRankUp;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	void LevelUpItem(FName ContainerId, FName TargetId, FName MaterialId);
	void RankUpItem(FName ContainerId, FName TargetId);

	void CanRankUp(FName ContainerId, FName TargetId, TFunction<void(bool)> Callback);

protected:

	UAssetManager* AssetManager;

	void HandleGameInitialized();

	bool HandleLevelUp(FName ContainerId, FName EnhanceableId, FEnhanceRecord EnhanceableRecord, UEnhanceableAsset* EnhanceableAsset, FName EnhanceId, int EnhancePoint);
	bool HandleRankUp(FName ContainerId, FName TargetId, FEnhanceRecord TargetEnhance, UEnhanceableAsset* TargetAsset, const TMap<FPrimaryAssetId, int>& ItemQuantities);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

};


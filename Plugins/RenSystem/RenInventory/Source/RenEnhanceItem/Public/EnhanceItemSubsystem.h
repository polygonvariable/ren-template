// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers

// Generated Headers
#include "EnhanceItemSubsystem.generated.h"

// Forward Declarations
class UAssetManager;

class UInventorySubsystem;
class IEnhanceProviderInterface;

struct FEnhanceRecord;



/**
 *
 */
UCLASS()
class UEnhanceItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	void LevelUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FPrimaryAssetId& MaterialAssetId, FName MaterialId);
	void RankUpItem(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId);

	void CanRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, TFunction<void(bool)> Callback);
	bool CanRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, IEnhanceProviderInterface* TargetAsset);
	bool CanRankUp(const FEnhanceRecord& EnhanceRecord, IEnhanceProviderInterface* TargetAsset);

protected:

	UAssetManager* AssetManager;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;


	void HandleGameInitialized();

	bool HandleLevelUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhancement, UObject* TargetObject, const FPrimaryAssetId& MaterialAssetId, FName MaterialId, UObject* MaterialObject);
	bool HandleRankUp(FName ContainerId, const FPrimaryAssetId& TargetAssetId, FName TargetId, const FEnhanceRecord& TargetEnhancement, UObject* TargetObject);

protected:

	// ~ UGameInstanceSubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~ End of UGameInstanceSubsystem

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemEnhance, FName /* TargetId */, bool /* bSuccess */);
	FOnItemEnhance OnItemLevelUp;
	FOnItemEnhance OnItemRankUp;

};


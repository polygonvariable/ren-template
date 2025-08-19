// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

// Project Headers
#include "RenCoreInventory/Public/InventoryRecord.h"

// Generated Headers
#include "EnhanceItemSubsystem.generated.h"

// Forward Declarations
class UEnhanceableAsset;
class UInventorySubsystem;



/**
 *
 */
UCLASS()
class UEnhanceItemSubsystem : public UGameInstanceSubsystem
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UFUNCTION(BlueprintCallable)
	bool LevelUpItem(FName ContainerId, FName ItemGuid, FName EnhanceGuid);

	UFUNCTION(BlueprintCallable)
	bool RankUpItem(FName ContainerId, FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	bool CanRankUp(FName ContainerId, FName ItemGuid) const;

protected:

	bool HandleLevelUp(UInventorySubsystem* Subsystem, FName ContainerId, FName ItemGuid, FName EnhanceGuid, const FInventoryRecord* ItemRecord, UEnhanceableAsset* ItemAsset, int EnhancePoint);
	bool HandleRankUp(UInventorySubsystem* Subsystem, FName ContainerId, FName ItemGuid, const FInventoryRecord* ItemRecord, UEnhanceableAsset* ItemAsset, const TMap<FName, int>& ItemQuantities);

	void HandleGameInitialized();

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};


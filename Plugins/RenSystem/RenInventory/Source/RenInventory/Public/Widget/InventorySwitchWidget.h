// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryBaseWidget.h"

// Generated Headers
#include "InventorySwitchWidget.generated.h"

// Forward Declarations
class UNamedSlot;
class UWidgetSwitcher;

class UFilterGroup;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Blueprintable)
class UInventorySwitchWidget : public UInventoryBaseWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	// ~ UInventoryBaseWidget
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	RENINVENTORY_API virtual void ResetDetails() override;
	// ~ End of UInventoryBaseWidget

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UNamedSlot> PrimarySlot = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UNamedSlot> SecondarySlot = nullptr;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BP_HandleSwitch(const FPrimaryAssetId& AssetId, FInventoryRecord Record);
	virtual bool BP_HandleSwitch_Implementation(const FPrimaryAssetId& AssetId, FInventoryRecord Record);

};


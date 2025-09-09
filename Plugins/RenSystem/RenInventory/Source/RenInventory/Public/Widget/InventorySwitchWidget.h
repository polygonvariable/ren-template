// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RCoreInventory/Public/InventoryFilterRule.h"

// Generated Headers
#include "InventorySwitchWidget.generated.h"

// Forward Declarations
class UNamedSlot;
class UWidgetSwitcher;

class UInventoryAsset;

struct FInventoryRecord;


/**
 *
 */
UCLASS(Blueprintable)
class UInventorySwitchWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryFilterRule FilterRule;

	RENINVENTORY_API void InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset);
	RENINVENTORY_API void ResetDetails();

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UNamedSlot> PrimarySlot = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UNamedSlot> SecondarySlot = nullptr;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool BP_HandleSwitch(FInventoryRecord Record, UInventoryAsset* Asset);
	virtual bool BP_HandleSwitch_Implementation(FInventoryRecord Record, UInventoryAsset* Asset);

};


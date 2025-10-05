// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "InventorySwitchUI.generated.h"

// Forward Declarations
class UNamedSlot;
class UWidgetSwitcher;

class UFilterGroup;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Blueprintable)
class UInventorySwitchUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	// ~ UInventoryUI
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity) override;
	// ~ End of UInventoryUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UNamedSlot> DetailSlot = nullptr;

	// ~ UInventoryUI
	virtual void SwitchDetails(bool bPrimary) override;
	// ~ End of UInventoryUI

};


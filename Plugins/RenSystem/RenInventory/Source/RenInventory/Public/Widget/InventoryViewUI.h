// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "InventoryViewUI.generated.h"

// Forward Declarations
class UInventoryCollectionUI;
class UInventoryDetailUI;
class UInventorySwitchUI;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UInventoryViewUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void DisplayEnhanceItem();

	void SetContainerId(FName ContainerId);

protected:

	UPROPERTY()
	TObjectPtr<UInventoryUI> EnhanceItemWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UInventoryUI> EnhanceItemClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> InventoryDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventorySwitchUI> InventorySwitch = nullptr;

	// ~ UInventoryBaseWidget
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	// ~ End of UInventoryBaseWidget

protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


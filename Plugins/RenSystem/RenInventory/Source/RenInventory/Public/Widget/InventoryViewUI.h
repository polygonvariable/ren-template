// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "InventoryViewUI.generated.h"

// Forward Declarations
class UButton;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	// ~ UInventoryUI
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	// ~ End of UInventoryUI

protected:

	UPROPERTY()
	TObjectPtr<UInventoryUI> EnhanceItemUI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UInventoryUI> EnhanceItemClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> ItemList = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> ItemDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UFUNCTION(BlueprintCallable)
	virtual void DisplayEnhanceItem();

	UFUNCTION(BlueprintNativeEvent)
	TArray<UInventorySwitchUI*> GetDetailSwitches() const;
	virtual TArray<UInventorySwitchUI*> GetDetailSwitches_Implementation() const;

protected:

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


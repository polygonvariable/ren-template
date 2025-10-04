// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "PurchaseItemUI.generated.h"

// Forward Declarations
class UInventoryCollectionUI;
class UInventoryDetailUI;

class UInventorySubsystem;
class UPurchaseItemSubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UPurchaseItemUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	// ~ UInventoryBaseWidget
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	virtual void ResetDetails() override;
	virtual void RefreshDetails() override;
	// ~ End of UInventoryBaseWidget

protected:

	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakObjectPtr<UPurchaseItemSubsystem> PurchaseItemSubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> InventoryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> RequiredCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> InventoryDetail = nullptr;

	UFUNCTION(BlueprintCallable)
	void PurchaseItem();

	int GetCraftingQuantity() const;

	void HandleLoadedAsset(UObject* LoadedAsset);

protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


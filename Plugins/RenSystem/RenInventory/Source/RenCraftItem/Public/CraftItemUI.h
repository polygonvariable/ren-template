// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Components/Button.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeRule.h"
#include "RenInventory/Public/Widget/InventoryUI.h"

#include "RenInventory/Public/Widget/InventoryEntryUI.h"

// Generated Headers
#include "CraftItemUI.generated.h"

// Forward Declarations
class UButton;

class UCounterSubsystem;

class UInventoryCollectionUI;
class UInventoryGlossaryUI;

class UInventorySubsystem;
class UCraftItemSubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UCraftItemUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	// ~ UInventoryUI
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	// ~ End of UInventoryUI

protected:

	FPrimaryAssetId ActiveAssetId;
	TWeakObjectPtr<UCounterSubsystem> CounterSubsystem;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakObjectPtr<UCraftItemSubsystem> CraftItemSubsystem;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName CraftId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bEnableCounter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FExchangeCatalog CraftCatalog;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CraftButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> CraftingList = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> RequiredList = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryGlossaryUI> ItemDetail = nullptr;


	UFUNCTION(BlueprintCallable)
	void DisplayStockItems();
	void DisplayRequiredItems(UObject* LoadedAsset);

	UFUNCTION(BlueprintCallable)
	void CraftItem();

	int GetCraftingQuantity() const;

	void HandleItemCrafted(bool bSuccess);

	// ~ UInventoryUI
	virtual void LockControls(bool bLock) override;
	virtual void CloseWidget() override;
	// ~ End of UInventoryUI

protected:

	// ~ UUserWidget
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



/**
 *
 */
UCLASS(Abstract)
class UCraftItemEntryUI : public UInventoryEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemLimit = nullptr;

	// ~ UInventoryEntryUI
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	virtual void SetTertiaryDetails(UInventoryEntry* Entry) override;
	// ~ End of UInventoryEntryUI

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"
#include "RCoreExchange/Public/ExchangeRule.h"

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


	// ~ UInventoryBaseWidget
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	// ~ End of UInventoryBaseWidget

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName CraftId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bEnableCounter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FExchangeCatalog CraftCatalog;

	FPrimaryAssetId ActiveAssetId;
	TWeakObjectPtr<UCounterSubsystem> CounterSubsystem;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakObjectPtr<UCraftItemSubsystem> CraftItemSubsystem;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CraftButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> CraftCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> RequiredCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryGlossaryUI> InventoryDetail = nullptr;


	UFUNCTION(BlueprintCallable)
	void DisplayStockItems();
	void DisplayRequiredItems(UObject* LoadedAsset);

	UFUNCTION(BlueprintCallable)
	void CraftItem();

	int GetCraftingQuantity() const;

	void HandleItemCrafted(bool bSuccess);

	void LockControls(bool bLock);
	void LockControls(TArray<UWidget*> Widgets, bool bLock);

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

	// ~ UInventoryBaseWidget
	virtual void SetTertiaryDetails(UInventoryEntry* Entry) override;
	// ~ End of UInventoryBaseWidget

};



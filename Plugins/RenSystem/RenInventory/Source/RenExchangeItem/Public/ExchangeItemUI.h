// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreExchange/Public/ExchangeQuota.h"

#include "RenInventory/Public/Widget/InventoryEntryUI.h"
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "ExchangeItemUI.generated.h"

// Forward Declarations
class UButton;
class UTextBlock;

class ICounterManagerInterface;
class UInventoryCollectionUI;
class UInventoryGlossaryUI;
class UInventorySubsystem;
class UExchangeItemSubsystem;

struct FInventoryRecord;
struct FExchangeRule;



/**
 *
 */
UCLASS(Abstract)
class UExchangeItemUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	// ~ UInventoryUI
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	// ~ End of UInventoryUI

protected:

	FPrimaryAssetId ActiveAssetId;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakInterfacePtr<ICounterManagerInterface> CounterManagerInterface;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName CounterId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bEnableCounter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FExchangeCatalog Catalog;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> CloseButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ExchangeButton = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> CatalogList = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> RequiredList = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryGlossaryUI> ItemDetail = nullptr;


	UFUNCTION(BlueprintCallable)
	void DisplayStockItems();
	void DisplayRequiredItems(UObject* LoadedAsset);

	UFUNCTION(BlueprintCallable)
	virtual void ExchangeItem();


	virtual int GetExchangeQuantity() const;
	virtual UExchangeItemSubsystem* GetExchangeItemSubsystem() const;
	virtual const FExchangeRule* GetExchangeRule(UObject* Target) const;


	// ~ Bindings
	virtual void HandleItemExchanged(bool bSuccess);
	// ~ End of Bindings

protected:

	// ~ UInventoryUI
	virtual void CloseWidget() override;
	// ~ End of UInventoryUI

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
class UExchangeItemEntryUI : public UInventoryEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemLimit = nullptr;

protected:

	// ~ UInventoryEntryUI
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	virtual void SetTertiaryDetails(UInventoryEntry* Entry) override;
	// ~ End of UInventoryEntryUI

};


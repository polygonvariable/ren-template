// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "InventoryEntryUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UInventoryEntry;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryEntryUI : public UInventoryUI, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UInventoryUI
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	RENINVENTORY_API virtual void ResetDetails() override;
	// ~ End of UInventoryUI

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;


	UFUNCTION(BlueprintCallable)
	void RemoveEntry();

	UFUNCTION(BlueprintCallable)
	void SelectEntry();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSelectionChanged(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleDetailsValidity(bool bIsValid);


	// ~ UInventoryUI
	RENINVENTORY_API virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image) override;
	RENINVENTORY_API virtual void SetSecondaryDetails(int Quantity) override;
	RENINVENTORY_API virtual void SetTertiaryDetails(UInventoryEntry* Entry) override;
	// ~ End of UInventoryUI

protected:

	// ~ IUserObjectListEntry
	RENINVENTORY_API virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	RENINVENTORY_API virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};



/**
 *
 */
UCLASS(Abstract)
class UInventoryQuantityUI : public UInventoryEntryUI
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemAmount = nullptr;

	// ~ UInventoryUI
	virtual void SetTertiaryDetails(UInventoryEntry* Entry) override;
	// ~ End of UInventoryUI

};


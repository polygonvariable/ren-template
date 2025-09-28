// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryBaseWidget.h"

// Generated Headers
#include "InventoryEntryWidget.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UInventoryEntryObject;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UInventoryEntryWidget : public UInventoryBaseWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

public:

	// ~ UInventoryBaseWidget
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	virtual void ResetDetails() override;
	// ~ End of UInventoryBaseWidget

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIconImage = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemTitleText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantityText = nullptr;


	UFUNCTION(BlueprintCallable)
	void RemoveEntry();

	UFUNCTION(BlueprintCallable)
	void SelectEntry();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSelectionChanged(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleDetailsValidity(bool bIsValid);


	// ~ UInventoryBaseWidget
	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image) override;
	virtual void SetSecondaryDetails(int Quantity) override;
	virtual void SetTertiaryDetails(UInventoryEntryObject* Entry) override;
	// ~ End of UInventoryBaseWidget

protected:

	// ~ IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;
	// ~ End of IUserObjectListEntry

};



/**
 *
 */
UCLASS(Abstract)
class UWInventoryEntryQuantity : public UInventoryEntryWidget
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemRequiredText = nullptr;

	// ~ UInventoryBaseWidget
	virtual void SetTertiaryDetails(UInventoryEntryObject* Entry) override;
	// ~ End of UInventoryBaseWidget

};


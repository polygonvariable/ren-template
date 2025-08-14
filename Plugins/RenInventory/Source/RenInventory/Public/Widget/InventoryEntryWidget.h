// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryBaseWidget.h"

// Generated Headers
#include "InventoryEntryWidget.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UInventoryAsset;
class UInventorySubsystem;
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

	virtual void InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset) override;
	virtual void ResetDetails() override;

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIconImage = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemGuidText = nullptr;

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


	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image) override;
	virtual void SetSecondaryDetails(const FText& Guid, int Quantity) override;
	virtual void SetTertiaryDetails(UInventoryEntryObject* Entry) override;

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;

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

	virtual void SetTertiaryDetails(UInventoryEntryObject* Entry) override;

};


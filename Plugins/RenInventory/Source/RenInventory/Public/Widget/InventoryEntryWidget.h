// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers

// Generated Headers
#include "InventoryEntryWidget.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;

class UInventoryEntryObject;



/**
 *
 */
UCLASS(Abstract)
class UInventoryEntryWidget : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

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
	void HandleEntrySelectionChanged(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleRecordValidity(bool bIsValid);

	virtual void HandleInventoryEntry(UInventoryEntryObject* Entry);

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;

};


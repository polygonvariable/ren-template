// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"

// Generated Headers
#include "InventoryWidget.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryCollectionWidget;
class UInventoryDetailWidget;
class UInventoryEntryObject;



/**
 *
 */
UCLASS(Abstract)
class UInventoryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionWidget> InventoryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailWidget> InventoryDetail = nullptr;


	void HandleItemSelected(FName Guid, const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


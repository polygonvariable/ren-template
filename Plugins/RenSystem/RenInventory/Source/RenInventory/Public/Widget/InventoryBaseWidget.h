// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "InventoryBaseWidget.generated.h"

// Forward Declarations
class UTexture2D;

class UInventoryAsset;
class UInventoryEntryObject;

struct FInventoryRecord;



/**
 *
 */
UCLASS(MinimalAPI, Abstract)
class UInventoryBaseWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	RENINVENTORY_API virtual void InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset) {};

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API virtual void RefreshDetails() {};

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API virtual void ResetDetails() {};

protected:

	RENINVENTORY_API virtual void SetPrimaryDetails(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image) {};
	RENINVENTORY_API virtual void SetSecondaryDetails(const FText& Guid, int Quantity) {};
	RENINVENTORY_API virtual void SetTertiaryDetails(UInventoryEntryObject* Entry) {};

};


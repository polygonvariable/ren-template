// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "InventoryBaseWidget.generated.h"

// Forward Declarations
class UAssetManager;
class UTexture2D;

class UInventoryAsset;
class UInventoryEntryObject;

struct FInventoryRecord;
struct FStreamableHandle;



/**
 *
 */
UCLASS(Abstract)
class RENINVENTORY_API UInventoryBaseWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record);

	UFUNCTION(BlueprintCallable)
	virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	virtual void ResetDetails();

protected:

	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image);
	virtual void SetSecondaryDetails(int Quantity);
	virtual void SetTertiaryDetails(UInventoryEntryObject* Entry);

protected:

	TObjectPtr<UAssetManager> AssetManager;

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "InventoryUI.generated.h"

// Forward Declarations
class UAssetManager;
class UTexture2D;

class UInventoryEntry;

struct FInventoryRecord;
struct FEnhanceRecord;



/**
 *
 */
UCLASS(Abstract)
class RENINVENTORY_API UInventoryUI : public UUserWidget
{

	GENERATED_BODY()

public:

	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity);
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record);

	UFUNCTION(BlueprintCallable)
	virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	virtual void ResetDetails();

protected:

	virtual void SwitchDetails(bool bPrimary);
	virtual void SwitchDetails(int Index);

	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image);

	virtual void SetSecondaryDetails(int Quantity);
	virtual void SetSecondaryDetails(int Quantity, const FEnhanceRecord& Enhance);

	virtual void SetTertiaryDetails(UInventoryEntry* Entry);

protected:

	TObjectPtr<UAssetManager> AssetManager;

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


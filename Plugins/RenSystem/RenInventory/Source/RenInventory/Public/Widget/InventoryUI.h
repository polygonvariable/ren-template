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
UCLASS(Abstract, MinimalAPI)
class UInventoryUI : public UUserWidget
{

	GENERATED_BODY()

public:

	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity);
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API virtual void RefreshDetails();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API virtual void ResetDetails();

protected:

	RENINVENTORY_API virtual void SwitchDetails(bool bPrimary);
	RENINVENTORY_API virtual void SwitchDetails(int Index);

	RENINVENTORY_API virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image);

	RENINVENTORY_API virtual void SetSecondaryDetails(int Quantity);
	RENINVENTORY_API virtual void SetSecondaryDetails(int Quantity, const FEnhanceRecord& Enhance);

	RENINVENTORY_API virtual void SetTertiaryDetails(UInventoryEntry* Entry);

	RENINVENTORY_API virtual void LockControls(bool bLock);
	RENINVENTORY_API void LockControls(TArray<UWidget*> Widgets, bool bLock);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API virtual void CloseWidget();

protected:

	TObjectPtr<UAssetManager> AssetManager;

	// ~ UUserWidget
	RENINVENTORY_API virtual void NativeConstruct() override;
	RENINVENTORY_API virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


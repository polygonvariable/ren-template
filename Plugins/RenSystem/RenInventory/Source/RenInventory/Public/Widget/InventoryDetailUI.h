// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "InventoryDetailUI.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;

class UInventorySubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UInventoryDetailUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = false;

	// ~ UInventoryUI
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	RENINVENTORY_API virtual void RefreshDetails() override;
	RENINVENTORY_API virtual void ResetDetails() override;
	// ~ End of UInventoryUI

protected:

	FPrimaryAssetId ActiveAssetId;
	FName ActiveRecordId = NAME_None;


	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> DetailSwitch = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemExperience = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemLevel = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemRank = nullptr;


	void FetchDetails(const FInventoryRecord* Record, int Quantity);

	// ~ UInventoryUI
	virtual void SwitchDetails(bool bPrimary) override;

	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image) override;
	virtual void SetSecondaryDetails(int Quantity, const FEnhanceRecord& Enhance) override;
	// ~ End of UInventoryUI

protected:

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


/**
 *
 */
UCLASS(Abstract)
class UInventoryGlossaryUI : public UInventoryUI
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = false;

	// ~ UInventoryUI
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity) override;
	RENINVENTORY_API virtual void RefreshDetails() override;
	RENINVENTORY_API virtual void ResetDetails() override;
	// ~ End of UInventoryUI

protected:

	FPrimaryAssetId ActiveAssetId;


	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> DetailSwitch = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription = nullptr;


	void FetchDetails(int Quantity);

	// ~ UInventoryUI
	virtual void SwitchDetails(bool bPrimary) override;

	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image) override;
	virtual void SetSecondaryDetails(int Quantity) override;
	// ~ End of UInventoryUI

protected:

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


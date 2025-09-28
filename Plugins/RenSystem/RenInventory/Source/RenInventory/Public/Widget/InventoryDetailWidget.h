// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryBaseWidget.h"

// Generated Headers
#include "InventoryDetailWidget.generated.h"

// Forward Declarations
class UImage;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UEditableTextBox;

class UInventorySubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UInventoryDetailWidget : public UInventoryBaseWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = false;

	// ~ UInventoryBaseWidget
	RENINVENTORY_API virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	RENINVENTORY_API virtual void RefreshDetails() override;
	RENINVENTORY_API virtual void ResetDetails() override;
	// ~ End of UInventoryBaseWidget

protected:

	UPROPERTY(BlueprintReadOnly)
	FPrimaryAssetId ActiveAssetId;

	UPROPERTY(BlueprintReadOnly)
	FName ActiveRecordId = NAME_None;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemImage = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemNameText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantityText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemDescriptionText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemRankText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemLevelText = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemExperienceText = nullptr;


	void HandleDetails(const FInventoryRecord* Record);

	// ~ UInventoryBaseWidget
	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, const TSoftObjectPtr<UTexture2D>& Image) override;
	virtual void SetSecondaryDetails(int Quantity) override;
	virtual void SetSecondaryDetails(int Quantity, int Rank, int Level, int Experience);
	// ~ End of UInventoryBaseWidget

protected:

	// ~ End of UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


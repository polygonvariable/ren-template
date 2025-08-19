// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

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

class UInventoryAsset;
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

	RENINVENTORY_API virtual void InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset) override;
	RENINVENTORY_API virtual void RefreshDetails() override;
	RENINVENTORY_API virtual void ResetDetails() override;

protected:

	UPROPERTY(BlueprintReadOnly)
	FName ActiveItemGuid = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryAsset> ActiveAsset = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UEditableTextBox> ItemGuidText = nullptr;

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

	virtual void SetPrimaryDetails(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image) override;
	virtual void SetSecondaryDetails(const FText& Guid, int Quantity) override;
	virtual void SetSecondaryDetails(const FText& Guid, int Quantity, int Rank, int Level, int Experience);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/InventoryRecord.h"

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



/**
 *
 */
UCLASS(Abstract)
class UInventoryDetailWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = true;

	void InitializeDetail(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset);
	void RefreshDetail();
	void ResetDetail();

protected:

	UPROPERTY(BlueprintReadOnly)
	FName ActiveItemGuid = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ContainerId = NAME_None;

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


	void HandleDetail(const FInventoryRecord* Record, UInventoryAsset* Asset);

	void SetPrimaryDetail(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image);
	void SetSecondaryDetail(int Quantity = 0, int Rank = 0, int Level = 0, int Experience = 0);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


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

	void InitializeDetail(const FName& Guid, const FInventoryRecord* Record, UInventoryAsset* Asset);
	void RefreshDetail();
	void ResetDetail();

protected:

	FName ItemGuid = NAME_None;

	UPROPERTY(EditAnywhere)
	FName ContainerId = NAME_None;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UEditableTextBox> ItemId = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemName = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantity = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemDescription = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemRank = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemLevel = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemExperience = nullptr;


	void HandleDetail(const FInventoryRecord* Record, UInventoryAsset* Asset);

	void SetPrimaryDetail(const FText& Title, const FText& Description, TSoftObjectPtr<UTexture2D> Image);
	void SetSecondaryDetail(int Quantity = 0, int Rank = 0, int Level = 0, int Experience = 0);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


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
	TObjectPtr<UWidgetSwitcher> DetailSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> AssetImage = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AssetTitle = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AssetDescription = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordRank = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordLevel = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordExperience = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> AssetTypeWidget = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TSet<TEnumAsByte<EInventoryItemType>> AssetTypeVisibility;


	void HandleDetail(const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


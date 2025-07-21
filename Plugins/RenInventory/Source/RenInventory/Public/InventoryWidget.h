// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/InventoryRecord.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventoryWidget.generated.h"

// Forward Declarations
class UImage;
class UListView;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;

class UInventoryAsset;
class UInventorySubsystem;



/**
 *
 */
UCLASS()
class UInventoryEntryObject : public UObject
{

	GENERATED_BODY()

public:

	FName ItemGuid = NAME_None;
	UInventoryAsset* InventoryAsset = nullptr;
	const FInventoryRecord* InventoryRecord = nullptr;

	void Reset()
	{
		ItemGuid = NAME_None;
		InventoryAsset = nullptr;
		InventoryRecord = nullptr;
	}

};



/**
 *
 */
UCLASS(Abstract)
class UInventoryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryEntryObject> EntryObjectClass = UInventoryEntryObject::StaticClass();

protected:

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> InventoryContainer = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryFilterRule FilterRule = FInventoryFilterRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryQueryRule QueryRule = FInventoryQueryRule();


	UFUNCTION(BlueprintCallable)
	virtual void DisplayItems();

	virtual void HandleDisplayOfEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleSelectedEntry(UObject* Object);

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemSelected, UInventoryEntryObject*, EntryObject);
	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemSelected OnInventoryItemSelected;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


/**
 *
 */
UCLASS(Abstract)
class UInventoryEntryWidget : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> ItemIconImage;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemTitleText;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ItemQuantityText;


	UFUNCTION(BlueprintCallable)
	void SelectInventoryEntry();

	virtual void HandleInventoryEntry(UInventoryEntryObject* Entry);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleEntrySelectionChanged(bool bSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleRecordValidity(bool bIsValid);

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bSelected) override;

};


/**
 *
 */
UCLASS(Abstract)
class UInventoryDetailWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction))
	void InitializeDetail(FInventoryRecord Record = FInventoryRecord(), FName RecordId = NAME_None, UInventoryAsset* Asset = nullptr);
	virtual void InitializeDetail_Implementation(FInventoryRecord Record = FInventoryRecord(), FName RecordId = NAME_None, UInventoryAsset* Asset = nullptr);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction))
	void RefreshDetail();
	virtual void RefreshDetail_Implementation();

protected:

	TObjectPtr<UInventorySubsystem> InventorySubsystem;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> DetailSwitcher;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UImage> AssetImage;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AssetTitle;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AssetDescription;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordRank;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordLevel;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> RecordExperience;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> AssetTypeWidget;


	UPROPERTY(BlueprintReadOnly)
	TSet<TEnumAsByte<EInventoryItemType>> AssetTypeVisibility;


	UPROPERTY(BlueprintReadWrite)
	FName InventoryRecordId;


	UPROPERTY(BlueprintReadWrite)
	FInventoryRecord InventoryRecord;


	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryAsset> InventoryAsset;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void HandleDetail();
	virtual void HandleDetail_Implementation();

};


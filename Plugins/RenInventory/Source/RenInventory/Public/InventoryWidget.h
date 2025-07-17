// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"
#include "RenCore/Public/Record/InventoryRecord.h"

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



struct FInventorySortEntry
{

	FName Guid = NAME_None;
	UInventoryAsset* Asset = nullptr;
	const FInventoryRecord* Record = nullptr;

	FInventorySortEntry(FName InGuid, UInventoryAsset* InAsset, const FInventoryRecord* InRecord) : Guid(InGuid), Asset(InAsset), Record(InRecord) {}

	void Reset()
	{
		Guid = NAME_None;
		Asset = nullptr;
		Record = nullptr;
	}

};



/**
 *
 */
UCLASS()
class UInventoryEntryObject : public UObject
{

	GENERATED_BODY()

public:

	FName Guid = NAME_None;
	UInventoryAsset* Asset = nullptr;
	FInventoryRecord Record = FInventoryRecord();

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
	TObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UListView> InventoryContainer;



	UFUNCTION(BlueprintCallable)
	void DisplayStoredRecords();

	void HandleDisplayOfEntry(UInventoryEntryObject* EntryObject);
	bool HandleEntryFiltering(const FInventoryRecord& InventoryRecord, UInventoryAsset* InventoryAsset);
	void HandleSelectedEntry(UObject* Object);

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
	TObjectPtr<UImage> AssetImage;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AssetTitle;


	UPROPERTY(BlueprintReadWrite)
	FName InventoryRecordId;


	UPROPERTY(BlueprintReadWrite)
	FInventoryRecord InventoryRecord;


	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventoryAsset> InventoryAsset;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void SelectEntry();
	virtual void SelectEntry_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void HandleEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleEntry_Implementation(UInventoryEntryObject* EntryObject);

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

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


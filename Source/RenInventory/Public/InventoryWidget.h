// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"
#include "RenCore/Public/Inventory/InventoryItemRarity.h"
#include "RenCore/Public/Inventory/InventoryItemType.h"

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
UCLASS(Blueprintable, DisplayName = "Inventory Entry Object")
class RENINVENTORY_API UInventoryEntryObject : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Inventory Entry Object")
	FName InventoryRecordId;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Inventory Entry Object")
	FInventoryRecord InventoryRecord;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Inventory Entry Object")
	UInventoryAsset* InventoryAsset;

};


/**
 *
 */
UCLASS(Abstract, DisplayName = "Inventory Widget")
class RENINVENTORY_API UInventoryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Widget|Properties")
	TSubclassOf<UInventoryEntryObject> EntryObjectClass = UInventoryEntryObject::StaticClass();

protected:

	// TMap<FName, TObjectPtr<UInventoryAsset>> CachedAssets;

	TObjectPtr<UInventorySubsystem> InventorySubsystem;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Widget|Binding")
	TObjectPtr<UListView> InventoryContainer;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Widget|Runtime")
	TMap<FName, FInventoryRecord> InventoryRecords;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Widget|Action")
	void DisplayStoredRecords(const bool bForceRefresh = false);
	virtual void DisplayStoredRecords_Implementation(const bool bForceRefresh = false);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Widget|Handler")
	void HandleDisplayOfEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleDisplayOfEntry_Implementation(UInventoryEntryObject* EntryObject);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Widget|Handler")
	bool HandleEntryFiltering(const FInventoryRecord InventoryRecord, UInventoryAsset* InventoryAsset);
	virtual bool HandleEntryFiltering_Implementation(const FInventoryRecord InventoryRecord, UInventoryAsset* InventoryAsset);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Widget|Handler")
	void HandleSelectedEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleSelectedEntry_Implementation(UInventoryEntryObject* EntryObject);

private:

	void HandleSelectedEntryCast(UObject* Object);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


/**
 *
 */
UCLASS(Abstract, DisplayName = "Inventory Entry Widget")
class RENINVENTORY_API UInventoryEntryWidget : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Entry Widget|Binding")
	TObjectPtr<UImage> AssetImage;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Entry Widget|Binding")
	TObjectPtr<UTextBlock> AssetTitle;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Entry Widget|Runtime")
	FName InventoryRecordId;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Entry Widget|Runtime")
	FInventoryRecord InventoryRecord;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Entry Widget|Runtime")
	TObjectPtr<UInventoryAsset> InventoryAsset;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Entry Widget|Action")
	void SelectEntry();
	virtual void SelectEntry_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Entry Widget|Handler")
	void HandleEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleEntry_Implementation(UInventoryEntryObject* EntryObject);

protected:

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};


/**
 *
 */
UCLASS(Abstract, DisplayName = "Inventory Detail Widget")
class RENINVENTORY_API UInventoryDetailWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Inventory Detail Widget|Action")
	void InitializeDetail(FInventoryRecord Record = FInventoryRecord(), FName RecordId = NAME_None, UInventoryAsset* Asset = nullptr);
	virtual void InitializeDetail_Implementation(FInventoryRecord Record = FInventoryRecord(), FName RecordId = NAME_None, UInventoryAsset* Asset = nullptr);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction), Category = "Inventory Detail Widget|Action")
	void RefreshDetail();
	virtual void RefreshDetail_Implementation();

protected:

	TObjectPtr<UInventorySubsystem> InventorySubsystem;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UWidgetSwitcher> DetailSwitcher;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UImage> AssetImage;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UTextBlock> AssetTitle;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UTextBlock> AssetDescription;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UTextBlock> RecordRank;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UTextBlock> RecordLevel;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UTextBlock> RecordExperience;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional), Category = "Inventory Detail Widget|Binding")
	TObjectPtr<UPanelWidget> AssetTypeWidget;


	UPROPERTY(BlueprintReadOnly, Category = "Inventory Detail Widget|Property")
	TSet<TEnumAsByte<EInventoryItemType>> AssetTypeVisibility;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Detail Widget|Runtime")
	FName InventoryRecordId;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Detail Widget|Runtime")
	FInventoryRecord InventoryRecord;


	UPROPERTY(BlueprintReadWrite, Category = "Inventory Detail Widget|Runtime")
	TObjectPtr<UInventoryAsset> InventoryAsset;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected), Category = "Inventory Detail Widget|Handler")
	void HandleDetail();
	virtual void HandleDetail_Implementation();

};


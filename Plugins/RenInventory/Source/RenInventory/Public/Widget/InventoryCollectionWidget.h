// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RenCore/Public/Record/InventoryRecord.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventoryCollectionWidget.generated.h"

// Forward Declarations
class UListView;

class UPersistentObjectPool;
class UInventorySubsystem;
class UInventoryEntryObject;



/**
 *
 */
UCLASS(Abstract)
class UInventoryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryEntryObject> EntryObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryFilterRule FilterRule = FInventoryFilterRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryQueryRule QueryRule = FInventoryQueryRule();


	UFUNCTION(BlueprintCallable)
	virtual void DisplayItems();

	UFUNCTION(BlueprintCallable)
	virtual void ClearItems();

protected:

	UPROPERTY()
	TObjectPtr<UPersistentObjectPool> EntryObjectPool = nullptr;

	UPROPERTY()
	TObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> InventoryContainer = nullptr;


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


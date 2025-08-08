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
class UInventoryCollectionWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UInventoryEntryObject> EntryObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FInventoryFilterRule FilterRule = FInventoryFilterRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FInventoryQueryRule QueryRule = FInventoryQueryRule();


	UFUNCTION(BlueprintCallable)
	virtual void DisplayItems();

	UFUNCTION(BlueprintCallable)
	virtual void ClearItems();

	UFUNCTION(BlueprintCallable)
	virtual void RefreshItems();

protected:

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> InventoryContainer = nullptr;


	virtual void HandleDisplayOfEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleSelectedEntry(UObject* Object);

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemSelected, FName /* ItemGuid */, const FInventoryRecord* /* Record */, UInventoryAsset* /* Asset */);
	FOnInventoryItemSelected OnItemSelected;

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


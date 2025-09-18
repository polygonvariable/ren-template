// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InstancedStruct.h"

// Project Headers
#include "RCoreInventory/Public/InventoryFilterRule.h"

#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventoryCollectionWidget.generated.h"

// Forward Declarations
class UListView;

class UPersistentObjectPool;
class UInventorySubsystem;
class UInventoryEntryObject;

struct FInventoryRecord;



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
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bEnablePayloads = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FInventoryFilterRule FilterRule = FInventoryFilterRule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FInventoryQueryRule QueryRule = FInventoryQueryRule();


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void DisplayItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void ClearItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void RefreshItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API UInventoryEntryObject* GetSelectedItem();


	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void AddPayload(FName ItemId, FInstancedStruct Payload);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void SetPayloads(const TMap<FName, FInstancedStruct>& Payloads);

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void ClearPayloads();

protected:

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> InventoryContainer = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FInstancedStruct> InventoryPayloads;


	virtual void ConstructEntry(const FPrimaryAssetId& AssetId, const FName& RecordId, const FInventoryRecord* Record);

	virtual void HandleDisplayOfEntry(UInventoryEntryObject* EntryObject);
	virtual void HandleSelectedEntry(UObject* Object);

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemSelected, const FPrimaryAssetId& /* AssetId */, FName /* RecordId */, const FInventoryRecord* /* Record */);
	FOnInventoryItemSelected OnItemSelected;

protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


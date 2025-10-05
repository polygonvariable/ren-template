// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InstancedStruct.h"

// Project Headers
#include "RenInventory/Public/InventoryDefinition.h"

// Generated Headers
#include "InventoryCollectionUI.generated.h"

// Forward Declarations
class UListView;

class UPoolSubsystem;

class UFilterGroup;
class UInventorySubsystem;
class UInventoryEntry;

class UFilterCriterion;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UInventoryCollectionUI : public UUserWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UInventoryEntry> EntryClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bEnablePayloads = false;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TObjectPtr<UFilterGroup> FilterRule = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FInventoryQueryRule QueryRule = FInventoryQueryRule();
	

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void DisplayItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void ClearItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API void RefreshItems();

	UFUNCTION(BlueprintCallable)
	RENINVENTORY_API UInventoryEntry* GetSelectedItem();


	RENINVENTORY_API void AddPayload(const FPrimaryAssetId& AssetId, FInstancedStruct Payload);
	RENINVENTORY_API void ClearPayloads();


	RENINVENTORY_API UFilterCriterion* GetCriterionByName(FName Name);
	
	template<typename T>
	T* GetCriterionByName(FName Name)
	{
		return Cast<T>(GetCriterionByName(Name));
	}

protected:

	TWeakObjectPtr<UPoolSubsystem> PoolSubsystem;
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	TMap<FPrimaryAssetId, FInstancedStruct> Payloads;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UListView> ItemList = nullptr;


	virtual void HandleSelectedItem(UObject* Object);

public:

	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemSelected, const FPrimaryAssetId& /* AssetId */, int /* Quantity */, const FInventoryRecord* /* Record */);
	FOnInventoryItemSelected OnItemSelected;

protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


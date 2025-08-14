// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers

// Generated Headers
#include "InventoryWidget.generated.h"

// Forward Declarations
class UInventoryAsset;
class UInventoryCollectionWidget;
class UInventoryDetailWidget;
class UInventorySwitchWidget;
class UInventoryBaseWidget;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UInventoryWidget : public UUserWidget
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void DisplayEnhanceItem();

protected:

	UPROPERTY()
	TObjectPtr<UInventoryBaseWidget> EnhanceItemWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	TSubclassOf<UInventoryBaseWidget> EnhanceItemClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionWidget> InventoryCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailWidget> InventoryDetail = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventorySwitchWidget> InventorySwitch = nullptr;

	void HandleItemSelected(FName Guid, const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


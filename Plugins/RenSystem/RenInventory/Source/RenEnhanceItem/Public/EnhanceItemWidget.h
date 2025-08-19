// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RenInventory/Public/Widget/InventoryBaseWidget.h"

// Generated Headers
#include "EnhanceItemWidget.generated.h"

// Forward Declarations
class UWidgetSwitcher;

class UInventoryAsset;
class UEnhanceableAsset;

class UInventoryCollectionWidget;
class UInventoryDetailWidget;
class UInventoryEntryObject;
class UInventorySubsystem;
class UEnhanceItemSubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UEnhanceItemWidget : public UInventoryBaseWidget
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	FName ContainerId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
	bool bAutoRefresh = false;


	virtual void InitializeDetails(const FName& ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset) override;
	virtual void ResetDetails() override;
	virtual void RefreshDetails() override;

protected:

	UPROPERTY(BlueprintReadOnly)
	FName ActiveItemGuid = NAME_None;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhanceableAsset> ActiveAsset = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;

	UPROPERTY();
	TWeakObjectPtr<UEnhanceItemSubsystem> EnhanceItemSubsystem;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> EnhanceSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionWidget> LevelUpCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionWidget> RankUpCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailWidget> InventoryDetail = nullptr;


	UFUNCTION(BlueprintCallable)
	void TryLevelUp(FName ItemGuid);

	UFUNCTION(BlueprintCallable)
	void TryRankUp();

	void HandleLevelUpDisplay();
	void HandleRankUpDisplay(const FInventoryRecord* Record);

	void HandleItemSelected(FName ItemGuid, const FInventoryRecord* Record, UInventoryAsset* Asset);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};


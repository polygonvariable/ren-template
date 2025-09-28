// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"
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

	// ~ UInventoryBaseWidget
	virtual void InitializeDetails(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record) override;
	virtual void ResetDetails() override;
	virtual void RefreshDetails() override;
	// ~ End of UInventoryBaseWidget

protected:

	FPrimaryAssetId ActiveAssetId = FPrimaryAssetId();
	FName ActiveItemId = NAME_None;
	FEnhanceRecord ActiveEnhanceRecord;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UEnhanceableAsset> ActiveAsset = nullptr;

	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
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
	void TryLevelUp(const FPrimaryAssetId& AssetId, FName ItemId);

	UFUNCTION(BlueprintCallable)
	void TryRankUp();


	void HandleAssetLoaded(FPrimaryAssetId AssetId, UObject* LoadedAsset);

	void HandleLevelUpDisplay();
	void HandleRankUpDisplay();

	void HandleItemSelected(const FPrimaryAssetId& AssetId, int Quantity, const FInventoryRecord* Record);


protected:

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


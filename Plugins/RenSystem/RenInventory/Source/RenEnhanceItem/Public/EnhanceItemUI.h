// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

// Project Headers
#include "RCoreEnhance/Public/EnhanceRecord.h"
#include "RenInventory/Public/Widget/InventoryUI.h"

// Generated Headers
#include "EnhanceItemUI.generated.h"

// Forward Declarations
class UWidgetSwitcher;

class IEnhanceProviderInterface;

class UInventoryCollectionUI;
class UInventoryDetailUI;
class UInventorySubsystem;

class UEnhanceItemSubsystem;

struct FInventoryRecord;



/**
 *
 */
UCLASS(Abstract)
class UEnhanceItemUI : public UInventoryUI
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
	FEnhanceRecord ActiveEnhancement;
	TWeakInterfacePtr<IEnhanceProviderInterface> ActiveAsset;


	TWeakObjectPtr<UInventorySubsystem> InventorySubsystem;
	TWeakObjectPtr<UEnhanceItemSubsystem> EnhanceItemSubsystem;


	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> EnhanceSwitch = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> LevelUpCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryCollectionUI> RankUpCollection = nullptr;

	UPROPERTY(BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UInventoryDetailUI> InventoryDetail = nullptr;
	

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


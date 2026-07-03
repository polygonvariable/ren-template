// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AscensionData.h"
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "InventoryAscensionDashboardUI.generated.h"

// Forward Declarations
class UAssetEntry;
class UAssetCollectionUI;
class UAssetDetailUI;
class UInventoryStorageManager;
class UInventoryAscensionSubsystem;
class UCoreDataAsset;
class IAscensionProvider;
struct FInventoryInstance;
struct FTaskResult;


/**
 *
 */
UCLASS(Abstract)
class UInventoryAscensionDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(EditAnywhere)
	bool bAutoRefresh = false;

	UPROPERTY()
	TObjectPtr<UInventoryAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UInventoryStorageManager> StorageManager = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> LevelItemCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> RankItemCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> InventoryDetail = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton = nullptr;

	FGuid ActiveItemId;
	FAscensionData AscensionInstance;
	const IAscensionProvider* AscensionProvider = nullptr;


	void ToggleAscension(const FInventoryInstance* Item);
	void ToggleLevelUp(const FInventoryInstance* Item);
	void ToggleRankUp(const FInventoryInstance* Item);

	UFUNCTION()
	void HandleLevelUp();

	UFUNCTION()
	void HandleRankUp();

	// ~ UAssetDashboardUI
	virtual void LockControls_Implementation() override;
	virtual void UnlockControls_Implementation() override;
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


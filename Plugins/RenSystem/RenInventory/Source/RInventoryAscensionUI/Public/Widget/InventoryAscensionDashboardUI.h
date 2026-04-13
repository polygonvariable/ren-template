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
class UInventoryStorage;
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
	TObjectPtr<UInventoryStorage> InventoryStorage = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> LevelItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> RankItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> InventoryDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton = nullptr;

	FGuid ActiveItemId;
	FAscensionData AscensionInstance;
	const IAscensionProvider* AscensionProvider = nullptr;


	void EnableControls();
	void DisableControls();

	void ToggleAscension(const FInventoryInstance* Item);
	void ToggleLevelUp(const FInventoryInstance* Item);
	void ToggleRankUp(const FInventoryInstance* Item);

	void HandleTaskCallback(const FTaskResult& Result);


	UFUNCTION()
	void HandleLevelUp();

	UFUNCTION()
	void HandleRankUp();

	// ~ UAssetDashboardUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AscensionData.h"
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "AvatarAscensionDashboardUI.generated.h"

// Forward Declarations
class UAssetEntry;
class UAssetCollectionUI;
class UAssetDetailUI;
class UAvatarStorageManager;
class UAvatarAscensionSubsystem;
class UCoreDataAsset;
class IAscensionProvider;
struct FAvatarInstance;
struct FTaskResult;


/**
 *
 */
UCLASS(Abstract)
class UAvatarAscensionDashboardUI : public UAssetDashboardUI
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
	TObjectPtr<UAvatarAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarStorageManager> StorageManager = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> LevelItemCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> RankItemCollection = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> AvatarDetail = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton = nullptr;

	FAscensionData AscensionInstance;
	const IAscensionProvider* AscensionProvider = nullptr;


	void ToggleAscension(const FAvatarInstance* Instance);
	void ToggleLevelUp(const FAvatarInstance* Instance);
	void ToggleRankUp(const FAvatarInstance* Instance);

	UFUNCTION()
	void HandleLevelUp();

	UFUNCTION()
	void HandleRankUp();

	// ~ UAssetDashboardUI
	virtual TArray<UWidget*> GetLockingControls_Implementation() const override;
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


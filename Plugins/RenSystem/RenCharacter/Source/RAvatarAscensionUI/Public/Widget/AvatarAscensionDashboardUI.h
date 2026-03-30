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
class UAvatarStorage;
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

	FAscensionData AscensionData;

	const IAscensionProvider* ActiveAscensionProvider = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarAscensionSubsystem> AscensionSubsystem = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAvatarStorage> AvatarStorage = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> LevelItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetCollectionUI> RankItemCollection = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UAssetDetailUI> AvatarDetail = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> LevelUpButton = nullptr;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> RankUpButton = nullptr;


	void EnableControls();
	void DisableControls();

	void ToggleAscension(const FAvatarInstance* Instance);
	void ToggleLevelUp(const FAvatarInstance* Instance);
	void ToggleRankUp(const FAvatarInstance* Instance);

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


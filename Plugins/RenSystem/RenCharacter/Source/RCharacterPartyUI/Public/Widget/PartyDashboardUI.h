// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetDashboardUI.h"

// Generated Headers
#include "PartyDashboardUI.generated.h"

// Forward Declarations
class UButton;
class UAssetCollectionUI;
class UAssetDetailUI;
class UAssetEntry;
class UCoreDataAsset;


/**
 *
 */
UCLASS(Abstract)
class UPartyDashboardUI : public UAssetDashboardUI
{

	GENERATED_BODY()

public:

	UFUNCTION()
	void SyncParty();

	// ~ UAssetDashboardUI
	virtual void InitializeDetail() override;
	// ~ End of UAssetDashboardUI

protected:

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> SyncButton = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetCollectionUI> AvatarCollection = nullptr;


	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


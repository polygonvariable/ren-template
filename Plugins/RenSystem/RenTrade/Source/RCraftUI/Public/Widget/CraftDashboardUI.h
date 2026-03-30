// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers

// Project Headers
#include "Widget/TradeDashboardUI.h"

// Generated Headers
#include "CraftDashboardUI.generated.h"

// Forward Declarations
class UAssetCollection;
class UCoreDataAsset;
class UCraftSubsystem;



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UCraftBuildDashboardUI : public UTradeDashboardUI
{

	GENERATED_BODY()

protected:

	UPROPERTY()
	TObjectPtr<UCraftSubsystem> CraftSubsystem;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> CraftButton = nullptr;


	UFUNCTION()
	virtual void HandleCraft();

	// ~ UTradeDashboardUI
	virtual const UAssetCollection* GetTradeMaterialCollection(const UCoreDataAsset* Asset) const override;
	// ~ End of UTradeDashboardUI

	// ~ UUserWidget
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};



/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UCraftClaimDashboardUI : public UCraftBuildDashboardUI
{

	GENERATED_BODY()

protected:

	// ~ UCraftDashboardUI
	virtual void HandleCraft() override;
	// ~ End of UCraftDashboardUI

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Widget/AssetDetailUI.h"

// Generated Headers
#include "AvatarDetailUI.generated.h"

// Forward Declarations
class UAscensionDetailUI;
class UAvatarStorageManager;
class UCoreDataAsset;
struct FAvatarInstance;


/**
 *
 */
UCLASS(Abstract)
class UAvatarDetailUI : public UAssetDetailUI
{

	GENERATED_BODY()

public:

	// ~ UAssetDetailUI
	virtual void InitializeDetail() override;
	virtual void RefreshDetail() override;
	// ~ End of UAssetDetailUI

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAscensionDetailUI> AscensionDetail = nullptr;

	UPROPERTY()
	TObjectPtr<UAvatarStorageManager> StorageManager = nullptr;


	virtual void SetCustomDetails(const FAvatarInstance* Instance);

	// ~ UAssetDetailUI
	virtual void SetPrimaryDetail(const UCoreDataAsset* Asset) override;
	virtual void SetSecondaryDetail(const UAssetEntry* Entry) override;
	// ~ End of UAssetDetailUI

	// ~ UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


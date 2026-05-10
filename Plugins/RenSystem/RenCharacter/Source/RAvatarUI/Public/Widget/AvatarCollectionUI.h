// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AvatarQueryType.h"
#include "Widget/AssetCollectionUI.h"

// Generated Headers
#include "AvatarCollectionUI.generated.h"

// Forward Declarations
class UAvatarStorageManager;


/**
 *
 */
UCLASS(Abstract)
class UAvatarCollectionUI : public UAssetCollectionUI
{

	GENERATED_BODY()

public:

	// ~ UAssetCollectionUI
	virtual void InitializeCollection() override;
	virtual void DisplayEntries() override;
	// ~ End of UAssetCollectionUI

protected:

	UPROPERTY(EditAnywhere)
	FAvatarQueryRule QueryRule;

	UPROPERTY()
	TObjectPtr<UAvatarStorageManager> StorageManager = nullptr;


	// ~ UUserWidget
	virtual void NativeDestruct() override;
	// ~ End of UUserWidget

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/Asset/MetadataAsset.h"

// Generated Headers
#include "AvatarInputBindingAsset.generated.h"

// Forward Declarations
class UInputAction;


/**
 *
 */
UCLASS(MinimalAPI)
class UAvatarInputBindingAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraPan = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> CameraZoom = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> Walk = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> Jump = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> Sprint = nullptr;


#if WITH_EDITOR
	// ~ UActorComponent
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UActorComponent
#endif

};


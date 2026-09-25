// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Data/Asset/MetadataAsset.h"

// Generated Headers
#include "HUDInputBindingAsset.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UInputAction;
class UUserWidget;


/**
 *
 */
USTRUCT()
struct FHUDInputBinding
{

	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere)
	FGuid InputId;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UUserWidget> WidgetClass;

	bool IsValid() const;

};


/**
 *
 */
UCLASS(MinimalAPI)
class UHUDInputBindingAsset : public UMetadataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FHUDInputBinding> Bindings;


	// ~ UMetadataAsset
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UMetadataAsset

#if WITH_EDITOR
	// ~ UMetadataAsset
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
	// ~ End of UMetadataAsset
#endif

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Object/AssetFragment.h"

// Generated Headers
#include "EquipmentFragment.generated.h"

// Forward Declarations
class UEquipmentDataDefinition;


/**
 *
 */
UCLASS()
class UEquipmentFragment : public UAssetFragment
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UEquipmentDataDefinition> EquipmentDefinition = nullptr;


	const UEquipmentDataDefinition* GetEquipmentDefinition() const;

#if WITH_EDITORONLY_DATA
	// ~ UAssetFragment
	virtual void AppendAssetBundleData(FAssetBundleData& Data) override;
	// ~ End of UAssetFragment
#endif

};


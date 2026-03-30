// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Asset/Category/EnhanceableAsset.h"
#include "Interface/IEquipmentProvider.h"

// Generated Headers
#include "EquipmentAsset.generated.h"



/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEquipmentAsset : public UEnhanceableAsset, public IEquipmentProvider
{

	GENERATED_BODY()

public:

	// ~ IEquipmentProvider
	virtual const TSoftClassPtr<AActor>& GetEquipmentClass() const override;
	virtual const TArray<FSoftClassPath>& GetEquipmentAbilities() const override;
	virtual const FName& GetEquipmentSocket() const override;
	// ~ End of IEquipmentProvider

protected:

	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Equipment", AllowAbstract = false))
	TSoftClassPtr<AActor> EquipmentClass;

	UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "Ability", AllowAbstract = false))
	TArray<FSoftClassPath> EquipmentAbilities;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

};


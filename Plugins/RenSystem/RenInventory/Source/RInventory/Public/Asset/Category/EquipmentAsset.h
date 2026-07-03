// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Asset/Category/EnhanceableAsset.h"
#include "Interface/EquipmentDataProvider.h"

// Generated Headers
#include "EquipmentAsset.generated.h"

// Forward Declaration
class UPrimaryDataAsset;


/**
 * 
 */
UCLASS(MinimalAPI, Abstract)
class UEquipmentAsset : public UEnhanceableAsset, public IEquipmentDataProvider
{

	GENERATED_BODY()

public:

	// ~ IEquipmentDataProvider
	//virtual const FSoftClassPath& GetEquipmentActorClass() const override;
	//virtual const TSoftObjectPtr<UPrimaryDataAsset>& GetEquipmentDataAsset() const override;
	// ~ End of IEquipmentDataProvider

protected:

	//UPROPERTY(EditDefaultsOnly, meta = (AssetBundles = "Equipment", MetaClass = "EquipmentActor", AllowAbstract = false))
	//FSoftClassPath EquipmentActorClass;
	
	//UPROPERTY()
	//TSoftObjectPtr<UPrimaryDataAsset> EquipmentDataAsset;


};


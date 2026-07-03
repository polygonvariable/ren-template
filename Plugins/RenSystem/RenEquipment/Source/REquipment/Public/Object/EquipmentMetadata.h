// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Object/AssetDataDefinition.h"
#include "Object/AssetFragment.h"

// Generated Headers
#include "EquipmentMetadata.generated.h"

// Forward Declarations
class AEquipmentActor;
class UEquipmentAbilityCollection;
class UEquipmentController;


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


/**
 *
 */
UCLASS(Const, Blueprintable)
class UEquipmentDataDefinition : public UAssetDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UEquipmentController> ControllerClass = nullptr;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AEquipmentActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UEquipmentAbilityCollection> AbilityCollection = nullptr;


#if WITH_EDITORONLY_DATA
	// ~ UAssetDataDefinition
	virtual void AppendAssetBundleData(FAssetBundleData& Data) override;
	// ~ End of UAssetDataDefinition
#endif

};

USTRUCT()
struct FEquipmentSocket
{
	
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere)
	FTransform SocketTransform = FTransform::Identity;

};

/**
 *
 */
UCLASS(Const, Blueprintable, meta = (DisplayName = "Equipment Data Definition (Weapon)"))
class UEquipmentDataDefinition_Weapon : public UEquipmentDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FEquipmentSocket EquipSocket;

	UPROPERTY(EditAnywhere)
	FEquipmentSocket UnequipSocket;

};




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Data/AssetDataDefinition.h"
#include "Core/Type/EquipmentSocketDefinition.h"

// Generated Headers
#include "EquipmentDataDefinition.generated.h"

// Forward Declarations
class AEquipmentActor;
class UEquipmentAbilityCollection;
class UEquipmentController;


/**
 *
 */
UCLASS(Const, Blueprintable)
class UEquipmentDataDefinition : public UAssetDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Equipment Detail", meta = (Categories = "Equipment.Category"))
	FGameplayTag CategoryTag;

	UPROPERTY(EditAnywhere, Category = "Equipment Detail")
	TSoftClassPtr<UEquipmentController> ControllerClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipment Detail")
	TSoftClassPtr<AEquipmentActor> ActorClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Equipment Detail")
	TSoftObjectPtr<UEquipmentAbilityCollection> AbilityCollection = nullptr;

	/** Tag that can be used to activate this equipment */
	UPROPERTY(EditAnywhere, Category = "Activation Tags")
	FGameplayTagContainer ActivationTag;

	/** Tag that can be used to deactivate this equipment */
	UPROPERTY(EditAnywhere, Category = "Activation Tags")
	FGameplayTagContainer DeactivationTag;

#if WITH_EDITORONLY_DATA
	// ~ UAssetDataDefinition
	virtual void AppendAssetBundleData(FAssetBundleData& Data) override;
	// ~ End of UAssetDataDefinition

	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

};


/**
 *
 */
UCLASS(Const, Blueprintable, meta = (DisplayName = "Equipment Data Definition (Weapon)"))
class UEquipmentDataDefinition_Weapon : public UEquipmentDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Weapon Attachment")
	FTransform AttachTransform;

	UPROPERTY(EditAnywhere, Category = "Weapon Attachment")
	FTransform DetachTransform;

	UPROPERTY(EditAnywhere, Category = "Attachment Socket")
	bool bOverrideSocket = false;

	UPROPERTY(EditAnywhere, Category = "Attachment Socket", meta = (EditCondition = "bOverrideSocket==true", EditConditionHides))
	FEquipmentSocketDefinition AttachSocket;

	UPROPERTY(EditAnywhere, Category = "Attachment Socket", meta = (EditCondition = "bOverrideSocket==true", EditConditionHides))
	FEquipmentSocketDefinition DetachSocket;

};


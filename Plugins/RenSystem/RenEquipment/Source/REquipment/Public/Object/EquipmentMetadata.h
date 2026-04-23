// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

// Project Headers
#include "Object/AssetFragment.h"
#include "Object/AssetDataDefinition.h"
#include "Definition/EquipmentData.h"

// Generated Headers
#include "EquipmentMetadata.generated.h"

// Forward Declarations
class UAnimInstance;
class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
class UCoreDataAsset;
class AEquipmentActor;
class IAssetInstanceCollection;
class IAscensionInstanceProvider;
class UEquipmentAbilityCollection;
class UEquipmentController;
struct FGameplayEventData;
struct FEquipmentTagData;


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


	const UEquipmentDataDefinition* GetEquipmentDefinitionCDO() const;

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
UCLASS(Const, Blueprintable, Meta = (DisplayName = "Equipment Data Definition (Weapon)"))
class UEquipmentDataDefinition_Weapon : public UEquipmentDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FEquipmentSocket EquipSocket;

	UPROPERTY(EditAnywhere)
	FEquipmentSocket UnequipSocket;

};




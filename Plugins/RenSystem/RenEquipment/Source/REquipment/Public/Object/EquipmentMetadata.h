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
struct FGameplayEventData;


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

/**
 *
 */
UCLASS(Const, Blueprintable, Meta = (DisplayName = "Equipment Data Definition (Weapon)"))
class UEquipmentDataDefinition_Weapon : public UEquipmentDataDefinition
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FTransform SocketTransform = FTransform::Identity;

	UPROPERTY(EditAnywhere)
	FName SocketName = NAME_None;

};

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class UEquipmentController : public UObject
{

	GENERATED_BODY()

public:


	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TObjectPtr<const UCoreDataAsset> EquipmentAsset;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	FEquipmentData EquipmentData;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	FGuid EquipmentOwnerId;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly)
	TObjectPtr<AEquipmentActor> EquipmentActor = nullptr;



	UFUNCTION(BlueprintCallable)
	bool GetIsEquipped() const;

	UFUNCTION(BlueprintCallable)
	void SetIsEquipped(bool IsEquipped);





	int GetEquipmentLevel() const;
	void SetEquipmentLevel(int Level);


	virtual bool InitializeController(const UEquipmentDataDefinition* DataDefinition);
	virtual void DeinitializeController();

	virtual void RefreshController();


	// ~ UObject
	virtual class UWorld* GetWorld() const override;
	virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

protected:


	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> EquipAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> UnequipAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> EquipmentAnimInstance = nullptr;


	UPROPERTY(VisibleAnywhere, AdvancedDisplay, BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;

	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;



	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;




	void ApplyAbilities();
	void RemoveAbilities();
	void RefreshAbilities();



	void HandleActivationEvent(const FGameplayEventData* Payload);



	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (DisplayName = "On Equipped"))
	void OnEquipped();
	virtual void OnEquipped_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (DisplayName = "On Unequipped"))
	void OnUnequipped();
	virtual void OnUnequipped_Implementation();


	UFUNCTION(BlueprintNativeEvent)
	void AttachEquipment();
	virtual void AttachEquipment_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void DetachEquipment();
	virtual void DetachEquipment_Implementation();


	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Initialize Controller"))
	void BP_InitializeController();
	virtual void BP_InitializeController_Implementation() {};

	UFUNCTION(BlueprintNativeEvent, Meta = (DisplayName = "Deinitialize Controller"))
	void BP_DeinitializeController();
	virtual void BP_DeinitializeController_Implementation() {};

private:

	UPROPERTY()
	bool _bIsEquipped = false;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TObjectPtr<const UEquipmentAbilityCollection> _AbilityCollection = nullptr;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TObjectPtr<const UEquipmentDataDefinition> _DataDefinition = nullptr;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	int _EquipmentLevel = 1;

};

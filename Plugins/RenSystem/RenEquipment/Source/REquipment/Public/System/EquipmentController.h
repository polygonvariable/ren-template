// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

// Project Headers
#include "Core/Type/EquipmentData.h"

// Generated Headers
#include "EquipmentController.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class ACharacter;
class UAnimInstance;
class UGameplayEffect;
class UGameplayAbility;
class UAbilitySystemComponent;
class UCoreDataAsset;
class AEquipmentActor;
class IAssetInstanceCollection;
class IAscensionInstanceProvider;
class UEquipmentAbilityCollection;
class UEquipmentDataDefinition;
class UEquipmentFragment;
struct FGameplayEventData;
struct FEquipmentTagData;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentController : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> EquipmentAsset;

	UPROPERTY()
	FEquipmentData EquipmentData;

	UPROPERTY()
	TObjectPtr<AEquipmentActor> EquipmentActor = nullptr;


	virtual bool InitializeController(const UEquipmentDataDefinition* InDataDefinition);
	virtual void DeinitializeController();

	virtual bool ActivateEquipment();
	virtual bool DeactivateEquipment(bool bForce = false);
	virtual void RefreshEquipment();

	REN_API const UEquipmentAbilityCollection* GetEquipmentAbilityCollection() const;
	REN_API const UEquipmentDataDefinition* GetEquipmentDataDefinition() const;
	REN_API AActor* GetEquipmentActor() const;
	REN_API int GetEquipmentLevel() const;

	bool IsInitialized() const;
	bool IsAttached() const;

	// ~ UObject
	virtual class UWorld* GetWorld() const override;
	// ~ End of UObject

#if WITH_EDITOR
	// ~ UObject
	virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY()
	TObjectPtr<const UEquipmentDataDefinition> DataDefinition = nullptr;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;


	virtual void InitializeAssetInstance();
	virtual void DeinitializeAssetInstance();

	virtual void InitializeGameplayEvent();
	virtual void DeinitializeGameplayEvent();

	virtual bool CanActivate() const;

	void CreateAbilities();
	void RemoveAbilities();
	void RefreshAbilities();

	virtual void AttachEquipment();
	virtual void DetachEquipment();
	
	virtual void OnControllerInitialized();
	virtual void OnControllerDeinitialized();

	UAnimInstance* GetOwnerAnimInstance() const;
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;
	AActor* GetEquipmentOwner() const;

	template<class T>
	T* GetEquipmentOwner() const
	{
		return Cast<T>(GetEquipmentOwner());
	}

	void SetEquipmentLevel(int Level);
	void SetIsAttached(bool bAttached);
	void SetIsInitialized(bool bInitialized);

private:

	UPROPERTY()
	mutable TObjectPtr<UAbilitySystemComponent> _OwnerAbilitySystem = nullptr;

	UPROPERTY()
	mutable TObjectPtr<UAnimInstance> _OwnerAnimInstance = nullptr;

	int _Level = 1;
	bool _bIsAttached = false;
	bool _bIsInitialized = false;

};


// Module Macros
#undef REN_API


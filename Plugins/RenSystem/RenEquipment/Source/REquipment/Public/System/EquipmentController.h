// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

// Project Headers
#include "Core/Type/EquipmentSpawnData.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "EquipmentController.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UAnimInstance;
class UAbilitySystemComponent;
class UCoreDataAsset;
class AEquipmentActor;
class IAssetInstanceCollection;
class IAscensionInstanceProvider;
class UEquipmentAbilityCollection;
class UEquipmentDataDefinition;


/**
 *
 */
UCLASS(Abstract, MinimalAPI, BlueprintType)
class UEquipmentController : public UObject
{

	GENERATED_BODY()

public:

	UPROPERTY()
	EDataSource SourceType = EDataSource::Static;


	bool InitializeController(const UCoreDataAsset* InEquipmentAsset, const FEquipmentInitializationData& InEquipmentData, AEquipmentActor* InEquipmentActor, const UEquipmentDataDefinition* InDataDefinition);
	void DeinitializeController();

	REN_API virtual bool ActivateEquipment();
	REN_API virtual bool DeactivateEquipment(bool bForce = false);
	REN_API virtual void RefreshEquipment();

	REN_API const UCoreDataAsset* GetEquipmentAsset() const;
	REN_API const FEquipmentInitializationData& GetEquipmentData() const;
	REN_API const UEquipmentAbilityCollection* GetEquipmentAbilityCollection() const;
	REN_API const UEquipmentDataDefinition* GetEquipmentDataDefinition() const;

	UFUNCTION(BlueprintCallable)
	REN_API AEquipmentActor* GetEquipmentActor() const;
	REN_API int GetEquipmentLevel() const;

	REN_API bool IsInitialized() const;
	REN_API bool IsAttached() const;

	// ~ UObject
	REN_API virtual class UWorld* GetWorld() const override final;
	// ~ End of UObject

#if WITH_EDITOR
	// ~ UObject
	REN_API virtual bool ImplementsGetWorld() const override final;
	// ~ End of UObject
#endif

protected:

	UPROPERTY()
	TObjectPtr<const UCoreDataAsset> EquipmentAsset = nullptr;

	UPROPERTY()
	FEquipmentInitializationData EquipmentData;

	UPROPERTY()
	TObjectPtr<AEquipmentActor> EquipmentActor = nullptr;

	UPROPERTY()
	TObjectPtr<const UEquipmentDataDefinition> DataDefinition = nullptr;

	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;


	void InitializeAssetInstance();
	void DeinitializeAssetInstance();

	REN_API virtual void InitializeGameplayEvent();
	REN_API virtual void DeinitializeGameplayEvent();

	REN_API virtual bool CanActivate() const;

	void CreateAbilities();
	void RemoveAbilities();
	void RefreshAbilities();

	REN_API virtual void AttachEquipment();
	REN_API virtual void DetachEquipment();
	
	REN_API virtual void OnControllerInitialized();
	REN_API virtual void OnControllerDeinitialized();

	REN_API UAnimInstance* GetOwnerAnimInstance() const;
	REN_API UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;
	REN_API AActor* GetEquipmentOwner() const;

	template<class T>
	T* GetEquipmentOwner() const
	{
		return Cast<T>(GetEquipmentOwner());
	}

	REN_API void SetEquipmentLevel(int Level);
	REN_API void SetIsAttached(bool bAttached);
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


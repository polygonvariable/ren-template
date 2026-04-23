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
#include "EquipmentController.generated.h"

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
UCLASS(Abstract, MinimalAPI, Blueprintable)
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


	const UEquipmentAbilityCollection* GetEquipmentAbilityCollection() const
	{
		return AbilityCollection;
	}
	const UEquipmentDataDefinition* GetEquipmentDataDefinition() const
	{
		return DataDefinition;
	}


	UFUNCTION(BlueprintCallable)
	virtual void ActivateEquipment();

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateEquipment();


	virtual bool InitializeController(const UEquipmentDataDefinition* InDataDefinition);
	virtual void DeinitializeController();

	// ~ UObject
	virtual class UWorld* GetWorld() const override;
	virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

protected:

	UPROPERTY()
	TObjectPtr<const UEquipmentAbilityCollection> AbilityCollection = nullptr;

	UPROPERTY()
	TObjectPtr<const UEquipmentDataDefinition> DataDefinition = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> EquipAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> UnequipAnimation = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> EquipmentAnimInstance = nullptr;


	const FEquipmentTagData* EquipmentTagData = nullptr;
	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;




	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;


	virtual void RefreshController();

	int GetEquipmentLevel() const;
	void SetEquipmentLevel(int Level);

	UAnimInstance* GetOwnerAnimInstance() const;
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;
	AActor* GetEquipmentOwner() const;
	AActor* GetEquipmentActor() const;


	template<class T>
	T* GetEquipmentOwner() const
	{
		return Cast<T>(GetEquipmentOwner());
	}

	void ApplyAbilities();
	void RemoveAbilities();
	void RefreshAbilities();

	virtual void HandleActivationEvent(const FGameplayEventData* Payload);
	void HandleAbilityActivation(const FGameplayEventData* Payload, FGameplayTag EventTag);

	virtual void OnEquipped();
	virtual void OnUnequipped();

	virtual void AttachEquipment();
	virtual void DetachEquipment();

	UFUNCTION(BlueprintNativeEvent)
	void OnControllerInitialized();
	virtual void OnControllerInitialized_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
	void OnControllerDeinitialized();
	virtual void OnControllerDeinitialized_Implementation() {};


	bool GetIsDeinitialized() const;

private:

	void SetIsDeinitialized(bool bState);

	bool _bIsDeinitialized = false;


	UPROPERTY()
	int _Level = 1;

	UPROPERTY()
	mutable TObjectPtr<UAbilitySystemComponent> _OwnerAbilitySystem = nullptr;

	UPROPERTY()
	mutable TObjectPtr<UAnimInstance> _OwnerAnimInstance = nullptr;

};


UENUM()
enum class EEquipmentState : uint8
{
	Equipped UMETA(DisplayName = "Equipped"),
	Unequipped UMETA(DisplayName = "Unequipped")
};



//UCLASS(Abstract, MinimalAPI, DefaultToInstanced)
//class UEquipmentControllerComponent : public UObject
//{
//
//	GENERATED_BODY()
//
//public:
//
//	virtual void RegisterComponent() {};
//	virtual void UnregisterComponent() {};
//
//};
//
//UCLASS(MinimalAPI, DefaultToInstanced)
//class UEquipmentControllerComponent_Projectile : public UEquipmentControllerComponent
//{
//
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditDefaultsOnly)
//	int MaxProjectileCount = 10;
//
//	UPROPERTY(Transient)
//	int CurrentProjectileCount = 10;
//
//};






/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, Meta = (DisplayName = "Equipment Controller (Weapon)"))
class UEquipmentController_Weapon : public UEquipmentController
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentDataChanged, int, int)
	FOnEquipmentDataChanged OnEquipmentDataChanged;


	virtual void ActivateEquipment() override;
	virtual void DeactivateEquipment() override;


	UFUNCTION(BlueprintCallable)
	int GetProjectileCount() const
	{
		return ProjectileCount;
	}

	UFUNCTION(BlueprintCallable)
	int GetMaxProjectileCount() const
	{
		return ProjectileCountMax;
	}

	UFUNCTION(BlueprintCallable)
	bool CanReloadProjectiles() const
	{
		return ProjectileCount < ProjectileCountMax;
	}

	UFUNCTION(BlueprintCallable)
	void ReloadProjectiles()
	{
		ProjectileCount = ProjectileCountMax;
		OnEquipmentDataChanged.Broadcast(ProjectileCount, ProjectileCountMax);
	}

	UFUNCTION(BlueprintCallable)
	int ConsumeProjectile()
	{
		if (ProjectileCount > 0)
		{
			ProjectileCount = FMath::Clamp(ProjectileCount - 1, 0, ProjectileCountMax);
			OnEquipmentDataChanged.Broadcast(ProjectileCount, ProjectileCountMax);
		}
		return ProjectileCount;
	}

	UFUNCTION(BlueprintCallable)
	bool ConsumeAndCheckProjectiles()
	{
		ConsumeProjectile();
		return HasEnoughProjectiles();
	}

	UFUNCTION(BlueprintCallable)
	bool HasEnoughProjectiles() const
	{
		return ProjectileCount > 0 || HasInfiniteProjectiles();
	}

	bool HasInfiniteProjectiles() const
	{
		return ProjectileCountMax == -1;
	}

	UPROPERTY(EditDefaultsOnly)
	int ProjectileCountMax = 10;

protected:


	int ProjectileCount = 10;

	EEquipmentState EquipmentState = EEquipmentState::Unequipped;

	virtual bool IsEquipped();

	virtual void HandleActivationEvent(const FGameplayEventData* Payload) override;

	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

	virtual void AttachEquipment() override;

	virtual bool LinkAnimInstance();
	virtual bool UnlinkAnimInstance();

	virtual bool PlayEquipAnimation();
	virtual bool PlayUnequipAnimation();

	virtual void HandleMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void HandleMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted);

	void RegisterAttachmentNotify(bool bIsEquip);
	void UnregisterAttachmentNotify(bool bIsEquip);
	virtual void HandleMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip);


	virtual void AttachToEquipSocket();
	virtual void AttachToUnequipSocket();

	virtual void OnControllerDeinitialized_Implementation() override;


};








/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, Meta = (DisplayName = "Equipment Controller (Skill)"))
class UEquipmentController_Skill : public UEquipmentController
{

	GENERATED_BODY()

public:

	virtual void HandleActivationEvent(const FGameplayEventData* Payload) override;

};












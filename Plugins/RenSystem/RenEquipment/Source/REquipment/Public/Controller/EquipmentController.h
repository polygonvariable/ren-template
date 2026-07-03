// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "Object/AssetFragment.h"
#include "Object/AssetDataDefinition.h"
#include "Definition/EquipmentData.h"

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


	UFUNCTION(BlueprintCallable)
	virtual void ActivateEquipment();

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateEquipment();

	virtual bool InitializeController(const UEquipmentDataDefinition* InDataDefinition);
	virtual void DeinitializeController();

	REN_API const UEquipmentAbilityCollection* GetEquipmentAbilityCollection() const;
	REN_API const UEquipmentDataDefinition* GetEquipmentDataDefinition() const;

	UFUNCTION(BlueprintCallable)
	REN_API AActor* GetEquipmentActor() const;

	// ~ UObject
	virtual class UWorld* GetWorld() const override;
	virtual bool ImplementsGetWorld() const override;
	// ~ End of UObject

protected:

	UPROPERTY()
	TObjectPtr<const UEquipmentAbilityCollection> AbilityCollection = nullptr;

	UPROPERTY()
	TObjectPtr<const UEquipmentDataDefinition> DataDefinition = nullptr;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	const FEquipmentTagData* EquipmentTagData = nullptr;
	IAssetInstanceCollection* InstanceCollection = nullptr;
	IAscensionInstanceProvider* InstanceAscension = nullptr;


	virtual void HandleActivationEvent(const FGameplayEventData* Payload); 

	UAnimInstance* GetOwnerAnimInstance() const;
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const;
	AActor* GetEquipmentOwner() const;

	template<class T>
	T* GetEquipmentOwner() const
	{
		return Cast<T>(GetEquipmentOwner());
	}

	int GetEquipmentLevel() const;
	void SetEquipmentLevel(int Level);

	virtual void RefreshController();

	void ApplyAbilities();
	void RemoveAbilities();
	void RefreshAbilities();

	virtual void OnEquipped();
	virtual void OnUnequipped();

	virtual void AttachEquipment();
	virtual void DetachEquipment();
	
	virtual void OnControllerInitialized();
	virtual void OnControllerDeinitialized();

	bool GetIsDeinitialized() const;

private:

	UPROPERTY()
	mutable TObjectPtr<UAbilitySystemComponent> _OwnerAbilitySystem = nullptr;

	UPROPERTY()
	mutable TObjectPtr<UAnimInstance> _OwnerAnimInstance = nullptr;

	int _Level = 1;
	bool _bIsDeinitialized = false;


	void SetIsDeinitialized(bool bState);

};


UENUM()
enum class EEquipmentAttachmentState : uint8
{
	Equipped UMETA(DisplayName = "Equipped"),
	Unequipped UMETA(DisplayName = "Unequipped")
};








/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller"))
class UEquipmentAttachmentController : public UEquipmentController
{

	GENERATED_BODY()

public:

	// ~ UEquipmentController
	virtual bool InitializeController(const UEquipmentDataDefinition* InDataDefinition) override;
	virtual void ActivateEquipment() override;
	virtual void DeactivateEquipment() override;
	// ~ End of UEquipmentController

#if WITH_EDITOR
	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StateEffectClass = nullptr;

	FActiveGameplayEffectHandle StateEffectHandle;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> EquipAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> UnequipAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> EquipmentAnimInstance = nullptr;

	EEquipmentAttachmentState AttachmentState = EEquipmentAttachmentState::Unequipped;


	// ~ UEquipmentController
	virtual void HandleActivationEvent(const FGameplayEventData* Payload) override;
	// ~ End of UEquipmentController

	virtual bool IsEquipped();

	// ~ UEquipmentController
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;
	// ~ End of UEquipmentController

	virtual bool LinkAnimInstance();
	virtual bool UnlinkAnimInstance();

	virtual bool PlayEquipAnimation();
	virtual bool PlayUnequipAnimation();

	virtual void RegisterAttachmentNotify(bool bIsEquip);
	virtual void UnregisterAttachmentNotify(bool bIsEquip);

	virtual void HandleMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip);
	virtual void HandleMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void HandleMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual void AttachToEquipSocket();
	virtual void AttachToUnequipSocket();

	// ~ UEquipmentController
	virtual void AttachEquipment() override;
	virtual void OnControllerDeinitialized() override;
	// ~ End of UEquipmentController

};



/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller (Weapon)"))
class UEquipmentWeaponController : public UEquipmentAttachmentController
{

	GENERATED_BODY()

public:

	DECLARE_MULTICAST_DELEGATE(FOnWeaponDataChanged)
	FOnWeaponDataChanged OnDataChanged;

};


/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller (Projectile Weapon)"))
class UEquipmentProjectileWeaponController : public UEquipmentWeaponController
{

	GENERATED_BODY()

public:


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
		OnDataChanged.Broadcast();
	}

	UFUNCTION(BlueprintCallable)
	int ConsumeProjectile()
	{
		if (ProjectileCount > 0)
		{
			ProjectileCount = FMath::Clamp(ProjectileCount - 1, 0, ProjectileCountMax);
			OnDataChanged.Broadcast();
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

};








/**
 *
 */
UCLASS(Abstract, MinimalAPI, Blueprintable, meta = (DisplayName = "Equipment Controller (Skill)"))
class UEquipmentSkillController : public UEquipmentController
{

	GENERATED_BODY()

protected:

	// ~ UEquipmentController
	virtual void HandleActivationEvent(const FGameplayEventData* Payload) override;
	// ~ End of UEquipmentController

};


// Module Macros
#undef REN_API


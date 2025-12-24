// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayEffectTypes.h"

// Project Headers
#include "Entity.h"

// Generated Headers
#include "CharacterEntity.generated.h"

// Forward Declarations
class USpringArmComponent;
class UCameraComponent;
class URAbilitySystemComponent;
class AWeaponTest;
class UGameplayEffect;

/**
 *
 */
//UENUM(BlueprintType, DisplayName = "Sub Movement Type")
//enum ESubMovement
//{
//	Idle UMETA(DisplayName = "Idle"),
//	Walk UMETA(DisplayName = "Walk"),
//	Sprint UMETA(DisplayName = "Sprint"),
//	Fall UMETA(DisplayName = "Fall")
//};




/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class ARCharacter : public AEntity, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{

	GENERATED_BODY()

public:

	ARCharacter();


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<URAbilitySystemComponent> RAbilitySystemComponent;




	// ~ IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	// ~ End of IGameplayTagAssetInterface


	// ~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ End of IAbilitySystemInterface

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CameraMaxZoom = 2000.0f;

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraPan(FVector2D Axis);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraZoom(float Delta, float Multiplier = 5.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void DirectionalMove(const FVector& Direction);
	virtual void DirectionalMove_Implementation(const FVector& Direction);


	UFUNCTION(BlueprintCallable)
	void DealDamage(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser);


	UFUNCTION(BlueprintCallable)
	void CancelAbility(FGameplayTagContainer WithTags);



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> OwnedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeaponTest> WeaponClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AWeaponTest> Weapon;

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();



	UFUNCTION(BlueprintCallable)
	bool IsMoving(float Threshold = 0.0f) const;



	//virtual void Tick(float DeltaTime) override;

};


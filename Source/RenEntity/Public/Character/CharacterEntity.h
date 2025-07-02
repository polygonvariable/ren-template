// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"

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
UCLASS(Abstract, DisplayName = "Character Entity")
class RENENTITY_API ACharacterEntity : public AEntity, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{

	GENERATED_BODY()

public:

	ACharacterEntity();


	//UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	//TEnumAsByte<ESubMovement> CurrentSubMovement = ESubMovement::Idle;
	//UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	//bool bCanSprint = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<URAbilitySystemComponent> RAbilitySystemComponent;


	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CameraMinZoom = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float CameraMaxZoom = 2000.0f;

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraPan(const FVector2D Axis);

	UFUNCTION(BlueprintCallable, Meta = (BlueprintProtected))
	void CameraZoom(const float Delta, const float Multiplier = 5.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void SimpleMove(FVector Direction);
	virtual void SimpleMove_Implementation(FVector Direction);


	UFUNCTION(BlueprintCallable)
	void DealDamage(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser);




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> OwnedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWeaponTest> WeaponClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AWeaponTest> Weapon;

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();


	//virtual void Tick(float DeltaTime) override;

};


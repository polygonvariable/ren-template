// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"

// Generated Headers
#include "RCharacterBase.generated.h"

// Module Macros
#define REN_API RCHARACTER_API

// Forward Declarations
class UAbilitySystemComponent;
class UCharacterAsset;
class UGameplayEffect;



/**
 *
 * 
 * 
 */
UCLASS(Abstract)
class REN_API ARCharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{

	GENERATED_BODY()

public:

	ARCharacterBase();


	DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);
	FOnCharacterDied OnCharacterDied;

	DECLARE_MULTICAST_DELEGATE(FOnCharacterRevived);
	FOnCharacterRevived OnCharacterRevived;


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	virtual bool IsAlive() const;


	virtual void InitializeCharacter(const UCharacterAsset* CharacterAsset);
	virtual void InitializeAttributes(const TMap<FGameplayTag, float>& Attributes);
	virtual void InitializeTags(const TMap<FGameplayTag, float>& Attributes);

	virtual void DeinitializeCharacter();



	// ~ ACharacter
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ End of ACharacter

	// ~ IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ End of IAbilitySystemInterface

	// ~ IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// ~ End of IGameplayTagAssetInterface

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitialAttributeEffectClass;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Meta = (ForceAsFunction, BlueprintProtected))
	void DirectionalMove(const FVector& Direction);
	virtual void DirectionalMove_Implementation(const FVector& Direction);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoving(float Threshold = 0.1f) const;

};


// Module Macros
#undef REN_API


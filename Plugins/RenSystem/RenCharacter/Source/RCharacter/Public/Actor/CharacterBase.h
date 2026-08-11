// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Core/Type/CharacterInitializationData.h"
#include "Interface/SpawnContextProvider.h"

// Generated Headers
#include "CharacterBase.generated.h"

// Module Macros
#define REN_API RCHARACTER_API

// Forward Declarations
class UCharacterTrajectoryComponent;
class UAbilitySystemComponent;
class UCharacterAsset;
class UGameplayEffect;


/**
 *
 */
UCLASS(Abstract)
class REN_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface, public ISpawnContextProvider
{

	GENERATED_BODY()

public:

	ACharacterBase();


	UPROPERTY(EditAnywhere)
	TObjectPtr<const UCharacterAsset>  CharacterAsset;

	UPROPERTY(EditAnywhere)
	FCharacterInitializationData CharacterData;



	DECLARE_MULTICAST_DELEGATE(FOnCharacterDied);
	FOnCharacterDied OnCharacterDied;

	DECLARE_MULTICAST_DELEGATE(FOnCharacterRevived);
	FOnCharacterRevived OnCharacterRevived;



	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UCharacterTrajectoryComponent> CharacterTrajectoryComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Default")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;


	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	virtual void InitializeCharacter();
	virtual void DeinitializeCharacter();


	UFUNCTION(BlueprintCallable)
	virtual UCharacterTrajectoryComponent* GetTrajectoryComponent() const;


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

	// ~ ISpawnContextProvider
	virtual void GetSpawnData(const FGameplayTag& InTag, FInstancedStruct& OutValue) const override;
	// ~ End of ISpawnContextProvider

	// ~ ACharacter
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of ACharacter

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> InitialAttributeEffectClass;


	virtual void RefreshCharacter();

	UFUNCTION(BlueprintCallable)
	void CallOnCharacterDied();

	UFUNCTION(BlueprintCallable)
	void CallOnCharacterRevived();


	virtual void InitializeAttributes();
	virtual void RefreshAttributes();

	virtual void AddDefaultAttributes();
	virtual void AddRuntimeAttributes();
	virtual void ApplyAttributes();


	virtual void InitializeTags();


	virtual void RegisterLifeStateEvent();
	virtual void UnregisterLifeStateEvent();


	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterInitialized();
	virtual void OnCharacterInitialized_Implementation();


	UFUNCTION(BlueprintCallable)
	int GetCharacterLevel() const;
	void SetCharacterLevel(int Level);

	TMap<FGameplayTag, float>& GetCharacterAttributes();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (ForceAsFunction, BlueprintProtected))
	void DirectionalMove(const FVector& Direction);
	virtual void DirectionalMove_Implementation(const FVector& Direction);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoving(float Threshold = 0.1f) const;

private:

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	int _CharacterLevel = 1;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TMap<FGameplayTag, float> _CharacterAttributes;

};


// Module Macros
#undef REN_API


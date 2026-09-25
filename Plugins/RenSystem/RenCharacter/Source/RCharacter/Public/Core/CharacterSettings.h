// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "CharacterSettings.generated.h"

// Forward Declarations
class UGameplayEffect;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Character"))
class UCharacterSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCharacterSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> CharacterBundles;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Effects")
	TSubclassOf<UGameplayEffect> DeadEffectClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health")
	FGameplayTag DeadStateTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataHealthTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataMaxHealthTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Level Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataLevelTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Damage Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataPhysicalDamageTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Defense Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataPhysicalDefenseTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Damage Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataElementalDamageTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Defense Tags", meta = (Categories = "Data.Attribute"))
	FGameplayTag DataElementalDefenseTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "State Tags")
	FGameplayTag StateDeadTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health")
	FGameplayTagContainer MovementModeTags;


	FGameplayTag ConvertMovementModeToTag(EMovementMode MovementMode) const;

	static RCHARACTER_API const UCharacterSettings* Get();

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "CharacterSettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Character"))
class UCharacterSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UCharacterSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly, Category = "Asset")
	TArray<FName> CharacterBundles;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Effects")
	TSubclassOf<UGameplayEffect> DeadEffectClass;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health")
	FGameplayTag DeadStateTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Tags")
	FGameplayTag DataHealthTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Health Tags")
	FGameplayTag DataMaxHealthTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Level Tags")
	FGameplayTag DataLevelTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Damage Tags")
	FGameplayTag DataPhysicalDamageTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Damage Tags")
	FGameplayTag DataElementalDamageTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "Defense Tags")
	FGameplayTag DataPhysicalDefenseTag;

	UPROPERTY(Config, EditDefaultsOnly, Category = "Defense Tags")
	FGameplayTag DataElementalDefenseTag;


	UPROPERTY(Config, EditDefaultsOnly, Category = "State Tags")
	FGameplayTag StateDeadTag;


	static const UCharacterSettings* Get()
	{
		return GetDefault<UCharacterSettings>();
	}

};


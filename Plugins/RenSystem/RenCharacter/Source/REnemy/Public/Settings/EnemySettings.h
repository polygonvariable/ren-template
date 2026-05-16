// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

// Generated Headers
#include "EnemySettings.generated.h"


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, Meta = (DisplayName = "RSystem - Character Enemy"))
class UEnemySettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEnemySettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Meta = (DisplayName = "Enemy Died Tag"))
	FGameplayTag ETag_EnemyDied;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyUpdateUI;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ETag_EnemyPartyDied;


	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Enemy Settings"))
	static const UEnemySettings* Get()
	{
		return GetDefault<UEnemySettings>();
	}

};


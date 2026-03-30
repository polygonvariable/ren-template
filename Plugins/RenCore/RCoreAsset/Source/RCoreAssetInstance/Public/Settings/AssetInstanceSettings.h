// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "AssetInstanceSettings.generated.h"


/**
 *
 */
UCLASS(Config = RenProject, DefaultConfig)
class UAssetInstanceSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UAssetInstanceSettings(const FObjectInitializer& ObjectInitializer)
	{
		CategoryName = TEXT("Ren Project");
	}

	UPROPERTY(Config, EditDefaultsOnly)
	TMap<FPrimaryAssetType, TSoftClassPtr<UGameInstanceSubsystem>> InstanceProviders;

	static const UAssetInstanceSettings* Get()
	{
		return GetDefault<UAssetInstanceSettings>();
	}

};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "EventflowSettings.generated.h"


/**
 *
 */
UCLASS(Config = RenProject, DefaultConfig, meta = (DisplayName = "RCore - Eventflow"))
class CEVENTFLOW_API UEventflowSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	UEventflowSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly)
	TMap<FPrimaryAssetType, TSoftClassPtr<USubsystem>> EngineProviders;


	static const UEventflowSettings* Get();

};


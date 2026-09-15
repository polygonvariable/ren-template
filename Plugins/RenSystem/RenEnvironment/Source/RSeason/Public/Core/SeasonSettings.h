// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "Engine/DeveloperSettings.h"

// Generated Headers
#include "SeasonSettings.generated.h"

// Forward Declarations
class USeasonSubsystem;


/**
 *
 */
UCLASS(MinimalAPI, Config = RenProject, DefaultConfig, meta = (DisplayName = "RSystem - Environment Season"))
class USeasonSettings : public UDeveloperSettings
{

	GENERATED_BODY()

public:

	USeasonSettings(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(Config, EditDefaultsOnly, Category = "Subsystem")
	TSubclassOf<USeasonSubsystem> SubsystemClass;

	/*
	 * RGB	- Color added
	 * A	- Color Multiplied
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceTint = TEXT("SeasonAttribute.Tint");

	/*
	 * S	- Specular
	 * R	- Roughness
	 * O	- Opacity
	 * W	- WPO
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceSROW = TEXT("SeasonAttribute.SROW");

	/*
	 * D	- Displacement
	 * C	- Coverage (The mask coverage)
	 * M	- Mask Type
	 * A	- Mask Add (Add value to mask of Specular & Roughness)
	 */
	UPROPERTY(Config, EditDefaultsOnly, Category = "Surface Property")
	FName SurfaceDCMA = TEXT("SeasonAttribute.DCMA");


	static const USeasonSettings* Get();

};


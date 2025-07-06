// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EnvironmentActor.h"
#include "Kismet/GameplayStatics.h"

// Engine Headers
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"

// Project Header
#include "Component/OrbitalLightComponent.h"


AEnvironmentActor::AEnvironmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Push(TEXT("Actor.Environment"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	if(IsValid(SceneComponent))
	{
		SceneComponent->SetupAttachment(GetRootComponent());

		SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
		if (IsValid(SkyLight))
		{
			SkyLight->SetupAttachment(SceneComponent);
			SkyLight->SetRealTimeCaptureEnabled(true);
			SkyLight->CubemapResolution = 16;
		}

		SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
		if (IsValid(SkyAtmosphere))
		{
			SkyAtmosphere->SetupAttachment(SceneComponent);
		}

		ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
		if (IsValid(ExponentialHeightFog))
		{
			ExponentialHeightFog->SetupAttachment(SceneComponent);
			ExponentialHeightFog->SetFogDensity(0.025f);
		}

		SunLight = CreateDefaultSubobject<UOrbitalLightComponent>(TEXT("Sun"));
		if (IsValid(SunLight))
		{
			SunLight->SetupAttachment(SceneComponent);
			SunLight->SetForwardShadingPriority(1);
			SunLight->SetAtmosphereSunLightIndex(0);
			SunLight->SetLightColor(FColor::FromHex("#FFFFFF"));
			SunLight->ComponentTags.Push(TEXT("Environment.Sun"));
		}

		MoonLight = CreateDefaultSubobject<UOrbitalLightComponent>(TEXT("Moon"));
		if (IsValid(MoonLight))
		{
			MoonLight->SetupAttachment(SceneComponent);
			MoonLight->SetSpecularScale(0.05f);
			MoonLight->SetIntensity(5.0f);
			MoonLight->SetLightSourceAngle(0.0f);
			MoonLight->SetForwardShadingPriority(0);
			MoonLight->SetAtmosphereSunLightIndex(1);
			MoonLight->SetDynamicShadowCascades(1);
			MoonLight->SetLightColor(FColor::FromHex("#4B6F91"));
			MoonLight->bInverseRotation = true;
			MoonLight->ComponentTags.Push(TEXT("Environment.Moon"));
		}

		SkyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkyMesh"));
		if (IsValid(SkyMesh))
		{
			SkyMesh->SetupAttachment(SceneComponent);
			SkyMesh->SetCastShadow(false);
			SkyMesh->SetCastContactShadow(false);
			SkyMesh->SetAffectDynamicIndirectLighting(false);
			SkyMesh->SetAffectDistanceFieldLighting(false);
			SkyMesh->SetCollisionProfileName(TEXT("NoCollision"));
			SkyMesh->PrimaryComponentTick.bStartWithTickEnabled = false;
		}

		MoonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoonMesh"));
		if (IsValid(MoonMesh))
		{
			MoonMesh->SetupAttachment(SceneComponent);
			MoonMesh->SetCastShadow(false);
			MoonMesh->SetCastContactShadow(false);
			MoonMesh->SetAffectDynamicIndirectLighting(false);
			MoonMesh->SetAffectDistanceFieldLighting(false);
			MoonMesh->SetCollisionProfileName(TEXT("NoCollision"));
			MoonMesh->PrimaryComponentTick.bStartWithTickEnabled = false;
		}
	}
}


void AEnvironmentActor::BeginPlay()
{
	Super::BeginPlay();
}

void AEnvironmentActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


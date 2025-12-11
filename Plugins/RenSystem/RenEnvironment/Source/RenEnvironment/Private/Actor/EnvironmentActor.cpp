// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EnvironmentActor.h"

// Engine Headers
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SplineComponent.h"

// Project Header
#include "RenEnvironment/Public/Component/OrbitalLightComponent.h"



AEnvironmentActor::AEnvironmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(TEXT("Actor.Environment"));
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);
		
		SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
		if (IsValid(SkyLight))
		{
			SkyLight->SetupAttachment(SceneComponent);

			SkyLight->CubemapResolution = 16;
			SkyLight->bLowerHemisphereIsBlack = false;
			SkyLight->OcclusionMaxDistance = 1500.0f;
			SkyLight->OcclusionCombineMode = EOcclusionCombineMode::OCM_Multiply;

			SkyLight->SetRealTimeCaptureEnabled(true);
			SkyLight->SetOcclusionContrast(0.5f);
			SkyLight->SetOcclusionExponent(1.6f);
			SkyLight->SetMinOcclusion(0.4f);
		}

		SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
		if (IsValid(SkyAtmosphere))
		{
			SkyAtmosphere->SetupAttachment(SceneComponent);
		}
		
		/*
		VolumetricCloud = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("VolumetricCloud"));
		if (IsValid(SkyLight))
		{
			VolumetricCloud->SetupAttachment(SceneComponent);
			VolumetricCloud->SetVisibility(false);
		}
		*/

		ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ExponentialHeightFog"));
		if (IsValid(ExponentialHeightFog))
		{
			ExponentialHeightFog->SetupAttachment(SceneComponent);
			ExponentialHeightFog->SetFogDensity(0.025f);
		}

		PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
		if (IsValid(PostProcess))
		{
			FPostProcessSettings& Settings = PostProcess->Settings;

			Settings.bOverride_Sharpen = 1;
			Settings.Sharpen = 1.25f;

			Settings.bOverride_DynamicGlobalIlluminationMethod = 1;
			Settings.DynamicGlobalIlluminationMethod = EDynamicGlobalIlluminationMethod::None;

			Settings.bOverride_LumenSceneLightingQuality = 1;
			Settings.LumenSceneLightingQuality = 0.25f;

			Settings.bOverride_LumenSceneDetail = 1;
			Settings.LumenSceneDetail = 0.25f;

			Settings.bOverride_LumenSceneViewDistance = 1;
			Settings.LumenSceneViewDistance = 5000.0f;

			Settings.bOverride_LumenFinalGatherQuality = 1;
			Settings.LumenFinalGatherQuality = 0.5f;

			Settings.bOverride_LumenFinalGatherScreenTraces = 1;
			Settings.LumenFinalGatherScreenTraces = 1;

			Settings.bOverride_LumenMaxTraceDistance = 1;
			Settings.LumenMaxTraceDistance = 6500.0f;

			Settings.bOverride_LumenSurfaceCacheResolution = 1;
			Settings.LumenSurfaceCacheResolution = 0.5f;

			Settings.bOverride_LumenSceneLightingUpdateSpeed = 1;
			Settings.LumenSceneLightingUpdateSpeed = 0.5f;

			Settings.bOverride_LumenFinalGatherLightingUpdateSpeed = 1;
			Settings.LumenFinalGatherLightingUpdateSpeed = 0.5f;

			Settings.bOverride_LumenDiffuseColorBoost = 1;
			Settings.LumenDiffuseColorBoost = 1.5f;

			Settings.bOverride_ReflectionMethod = 1;
			Settings.ReflectionMethod = EReflectionMethod::ScreenSpace;
			
			Settings.bOverride_LumenReflectionQuality = 1;
			Settings.LumenReflectionQuality = 0.25f;

			Settings.bOverride_LumenReflectionsScreenTraces = 1;
			Settings.LumenReflectionsScreenTraces = 0;

			Settings.bOverride_ScreenSpaceReflectionQuality = 1;
			Settings.ScreenSpaceReflectionQuality = 35.0f;

			Settings.bOverride_AmbientOcclusionIntensity = 1;
			Settings.AmbientOcclusionIntensity = 0.65f;

			Settings.bOverride_AmbientOcclusionRadius = 1;
			Settings.AmbientOcclusionRadius = 100.0f;
		}

		SunLight = CreateDefaultSubobject<UOrbitalLightComponent>(TEXT("Sun"));
		if (IsValid(SunLight))
		{
			SunLight->SetupAttachment(SceneComponent);

			SunLight->CastTranslucentShadows = false;
			SunLight->CastStaticShadows = false;
			SunLight->bCastVolumetricShadow = false;
			SunLight->bAffectTranslucentLighting = false;

			SunLight->bEnableLightShaftOcclusion = true;
			SunLight->bEnableLightShaftBloom = true;
			SunLight->BloomThreshold = 1.0f;

			SunLight->EnabledStartTime = 7.25f;
			SunLight->EnabledEndTime = 20.25f;

			SunLight->SetIntensity(10.0f);
			SunLight->SetForwardShadingPriority(1);
			SunLight->SetAtmosphereSunLightIndex(0);
			SunLight->SetLightColor(FColor::FromHex("#FFFFFF"));

			SunLight->ComponentTags.Push(TEXT("Environment.Sun"));
		}

		MoonLight = CreateDefaultSubobject<UOrbitalLightComponent>(TEXT("Moon"));
		if (IsValid(MoonLight))
		{
			MoonLight->SetupAttachment(SceneComponent);

			MoonLight->CastTranslucentShadows = false;
			MoonLight->CastStaticShadows = false;
			MoonLight->bCastVolumetricShadow = false;
			MoonLight->bAffectTranslucentLighting = false;

			MoonLight->bEnableLightShaftOcclusion = true;
			MoonLight->bEnableLightShaftBloom = true;
			MoonLight->BloomThreshold = 0.025f;

			MoonLight->EnabledStartTime = 19.25f;
			MoonLight->EnabledEndTime = 8.25f;

			MoonLight->bInverseRotation = true;

			MoonLight->SetSpecularScale(0.05f);
			MoonLight->SetIntensity(5.0f);
			MoonLight->SetLightSourceAngle(0.0f);
			MoonLight->SetForwardShadingPriority(0);
			MoonLight->SetAtmosphereSunLightIndex(1);
			MoonLight->SetDynamicShadowCascades(1);
			MoonLight->SetLightColor(FColor::FromHex("#4B6F91"));

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


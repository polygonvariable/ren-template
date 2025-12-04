// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Generated Headers
#include "EnvironmentActor.generated.h"

// Forward Declarations
class USceneComponent;
class USkyLightComponent;
class UVolumetricCloudComponent;
class USkyAtmosphereComponent;
class UExponentialHeightFogComponent;
class UStaticMeshComponent;
class UPostProcessComponent;
class USplineComponent;
class ALandscape;
class ALandscapeSplineActor;
class ALandscapeProxy;
class ULandscapeSplineControlPoint;

class UOrbitalLightComponent;



/**
 * 
 * 
 * 
 */
UCLASS()
class AEnvironmentActor : public AActor
{

	GENERATED_BODY()
	
public:

	AEnvironmentActor();


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UVolumetricCloudComponent> VolumetricCloud;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFog;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UPostProcessComponent> PostProcess;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UOrbitalLightComponent> SunLight;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UOrbitalLightComponent> MoonLight;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> SkyMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MoonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = "0", UIMax = "24", ClampMin = "0", ClampMax = "24"))
	float TimeOfDay = 0.0f;

protected:

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor


};


/**
 *
 * TODO
 * extend Landscape Spline Actor
 *
 */
UCLASS()
class ASplineTest : public AActor
{

	GENERATED_BODY()

public:

	ASplineTest();


	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ALandscape> Landscape = nullptr;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<ALandscapeSplineActor> SplineActor = nullptr;

	UPROPERTY(EditAnywhere)
	ALandscapeProxy* LandscapeProxy = nullptr;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateForLandscape();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateViaActor();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateSpline();

	void AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd);


	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateEditorSpline();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateLandscapeSpline();

};

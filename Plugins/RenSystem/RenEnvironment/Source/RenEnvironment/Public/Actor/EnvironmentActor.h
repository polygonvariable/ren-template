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


UENUM(BlueprintType)
enum class ESegmentType : uint8
{
	Spline UMETA(DisplayName = "Spline"),
	Socket UMETA(DisplayName = "Socket")
};

UENUM(BlueprintType)
enum class ESplinePositionType : uint8
{
	Start UMETA(DisplayName = "Start"),
	End UMETA(DisplayName = "End"),
	Custom UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FSegmentData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESegmentType SegmentType = ESegmentType::Spline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESplinePositionType PointPosition = ESplinePositionType::Start;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TangentMultiplier = 250.0f;

};



/**
 *
 *
 */
UCLASS()
class ASplineSegment : public AActor
{

	GENERATED_BODY()

public:

	ASplineSegment();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	TSoftObjectPtr<ALandscapeSplineActor> LandscapeSpline;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> SegmentMesh;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Meta = (ContentDir))
	FDirectoryPath ExportPath;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	FString ExportName = TEXT("Segment");

	UPROPERTY(EditAnywhere)
	bool bAutoUpdate = false;

	UPROPERTY(EditAnywhere)
	bool bEnableMeshBuild = false;

	UPROPERTY(EditAnywhere)
	bool bEnableSnap = false;

	UPROPERTY(EditAnywhere)
	bool bEnableSnapRotation = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bClearSplineMeshAfterBuild = false;

	UPROPERTY(EditAnywhere)
	float SnapOffset = 10.0f;

	UPROPERTY(EditAnywhere)
	FSegmentData StartPoint;

	UPROPERTY(EditAnywhere)
	FSegmentData EndPoint;

#endif

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

#if WITH_EDITOR

	UFUNCTION(BlueprintCallable, CallInEditor)
	void BuildSpline();



	UFUNCTION(BlueprintCallable, CallInEditor)
	void BuildSplineMeshes();

	UFUNCTION(BlueprintCallable)
	void RemoveSplineMeshes();



	UFUNCTION(BlueprintCallable, CallInEditor)
	void SnapSplineToSurface(bool bEnableRotation);



	UFUNCTION(BlueprintCallable, CallInEditor)
	void RecenterActor();


	UFUNCTION(BlueprintCallable, CallInEditor)
	void BuildLandscapeSpline();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void BakeSplineMesh();



	void AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd);

	bool GetSplineData(USplineComponent* InSpline, ESplinePositionType InPointPosition, FVector& OutLocation, FRotator& OutRotation, FVector& OutTangent);

	// ~ AActor
	virtual void OnConstruction(const FTransform& Transform) override;
	// ~ End of AActor

#endif

};


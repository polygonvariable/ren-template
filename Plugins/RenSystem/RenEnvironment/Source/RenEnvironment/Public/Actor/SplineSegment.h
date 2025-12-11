// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Generated Headers
#include "SplineSegment.generated.h"

// Forward Declarations
class USceneComponent;

#if WITH_EDITOR

class USplineComponent;
class ALandscapeSplineActor;
class ULandscapeSplineControlPoint;

#endif



#if WITH_EDITORONLY_DATA

/**
 *
 *
 */
UENUM()
enum class ESegmentType : uint8
{
	Spline UMETA(DisplayName = "Spline"),
	Socket UMETA(DisplayName = "Socket")
};

/**
 *
 *
 */
UENUM()
enum class ESegmentSplinePosition : uint8
{
	Start UMETA(DisplayName = "Start"),
	End UMETA(DisplayName = "End")
};

/**
 *
 *
 */
USTRUCT()
struct FSegmentData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bEnabled = false;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "bEnabled"))
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "bEnabled"))
	ESegmentType SegmentType = ESegmentType::Socket;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "bEnabled && SegmentType==ESegmentType::Spline", EditConditionHides))
	ESegmentSplinePosition SplinePosition = ESegmentSplinePosition::Start;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "bEnabled && SegmentType==ESegmentType::Socket", EditConditionHides))
	FName SocketName = NAME_None;

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "bEnabled && SegmentType==ESegmentType::Socket", EditConditionHides))
	float TangentSize = 250.0f;

};

#endif



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

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

#if WITH_EDITORONLY_DATA

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	TObjectPtr<USplineComponent> SplineComponent;


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


	// ~ AActor
	virtual void OnConstruction(const FTransform& Transform) override;
	// ~ End of AActor

protected:

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
	bool bBuildMeshByPoints = true;

	UPROPERTY(EditAnywhere)
	bool bEnableSnap = false;

	UPROPERTY(EditAnywhere)
	bool bEnableSnapRotation = false;

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bClearMeshAfterBuild = true;

	UPROPERTY(EditAnywhere)
	float SnapOffset = 10.0f;

	UPROPERTY(EditAnywhere)
	float SegmentSize = 200.0f;

	UPROPERTY(EditAnywhere)
	float SideOffset = 0.0f;

	UPROPERTY(EditAnywhere)
	float VerticalOffset = 0.0f;

	UPROPERTY(EditAnywhere)
	FSegmentData StartPoint;

	UPROPERTY(EditAnywhere)
	FSegmentData EndPoint;


	void BuildSplineMeshByPoints();
	void BuildSplineMeshByDistance();

	void JoinSplineToSocket(const FSegmentData& SegmentData, int Index, float Direction);
	void JoinSplineToSpline(const FSegmentData& SegmentData, int Index, float StartDirection, float EndDirection);

	void AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd);

	bool GetSplineData(USplineComponent* InSpline, ESegmentSplinePosition InSplinePosition, FVector& OutLocation, FRotator& OutRotation, FVector& OutTangent);

#endif

};


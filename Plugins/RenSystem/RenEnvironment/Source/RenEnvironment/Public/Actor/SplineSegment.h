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

/**
 *
 *
 */
USTRUCT()
struct FSegmentMesh
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditAnywhere)
	float SideOffset = 0.0f;

	UPROPERTY(EditAnywhere)
	float VerticalOffset = 0.0f;

	UPROPERTY(EditAnywhere)
	float Size = 200.0f;

	bool IsValid() const
	{
		return StaticMesh != nullptr && Size > 0.0f;
	}

};

#endif



/**
 * 
 * Actor for generating spline-based geometry such as roads, rails etc.
 * 
 * Notes:
 * - Avoid using high-poly meshes, as they can negatively impact performance.
 * - Generated meshes must be baked; otherwise, they will not appear in cooked (packaged) builds.
 * - SplineComponent is only available in editor.
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


	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Segment")
	void BuildSpline();


	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Mesh")
	void BuildSplineMeshes();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Mesh")
	void RemoveSplineMeshes();


	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Segment")
	void SnapSplineToSurface(bool bEnableRotation);


	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Segment")
	void RecenterActor();


	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Mesh")
	void RemoveStaticMeshComponents();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Landscape")
	void BuildLandscapeSpline();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Export")
	void BakeSplineMesh();


	// ~ AActor
	virtual void OnConstruction(const FTransform& Transform) override;
	// ~ End of AActor

protected:

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TArray<FSegmentMesh> SegmentMeshes;

	UPROPERTY(EditAnywhere, Category = "Landscape")
	TSoftObjectPtr<ALandscapeSplineActor> LandscapeSpline;

	UPROPERTY(EditAnywhere, Category = "Export", Meta = (ContentDir))
	FDirectoryPath ExportPath;

	UPROPERTY(EditAnywhere, Category = "Export")
	FString ExportName = TEXT("Segment");

	UPROPERTY(EditAnywhere, Category = "Segment")
	bool bAutoUpdate = false;

	UPROPERTY(EditAnywhere, Category = "Segment", Meta = (EditCondition = "bAutoUpdate"))
	bool bEnableMeshBuild = false;
	
	UPROPERTY(EditAnywhere, Category = "Segment", Meta = (EditCondition = "bAutoUpdate && bEnableMeshBuild", EditConditionHides))
	bool bBuildMeshByPoints = true;

	UPROPERTY(EditAnywhere, Category = "Segment", Meta = (EditCondition = "bAutoUpdate"))
	bool bEnableSnap = false;

	UPROPERTY(EditAnywhere, Category = "Segment", Meta = (EditCondition = "bAutoUpdate && bEnableSnap", EditConditionHides))
	bool bEnableSnapRotation = false;

	UPROPERTY(EditAnywhere, Category = "Segment", Meta = (EditCondition = "bAutoUpdate && bEnableSnap", EditConditionHides))
	float SnapOffset = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Export")
	bool bClearSplineMeshesAfterBake = true;

	UPROPERTY(EditAnywhere, Category = "Export")
	bool bCombineBakedMesh = true;

	UPROPERTY(EditAnywhere, Category = "Segment")
	FSegmentData StartPoint;

	UPROPERTY(EditAnywhere, Category = "Segment")
	FSegmentData EndPoint;


	void BuildSplineMeshByPoints();
	void BuildSplineMeshByDistance();

	void JoinSplineToSocket(const FSegmentData& SegmentData, int Index, float Direction);
	void JoinSplineToSpline(const FSegmentData& SegmentData, int Index, float StartDirection, float EndDirection);

	void AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd);

	bool GetSplineData(USplineComponent* InSpline, ESegmentSplinePosition InSplinePosition, FVector& OutLocation, FRotator& OutRotation, FVector& OutTangent);



	void AddStaticMeshComponent(UObject* MeshObject);

	void BakePrimitiveComponents(FString Name, TArray<UPrimitiveComponent*> MeshComponents, TArray<UObject*>& OutObjects);

	void BakeSplineMeshSeparately();
	void BakeSplineMeshIntoSingle();


#endif

};


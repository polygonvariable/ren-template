// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EnvironmentActor.h"

// Engine Headers
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/SplineComponent.h"
#include "EngineUtils.h"
#include "Landscape.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineControlPoint.h"
#include "LandscapeSplineSegment.h"
#include "LandscapeSplineActor.h"

#include "StaticMeshEditorSubsystem.h"
#include "StaticMeshEditorSubsystemHelpers.h"
#include "UnrealEdGlobals.h"

#include "IMeshMergeUtilities.h"
#include "MeshMergeModule.h"
#include "FileHelpers.h"
#include "EditorAssetLibrary.h"

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

ASplineTest::ASplineTest()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);

		SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
		if (IsValid(SplineComponent))
		{
			SplineComponent->SetupAttachment(SceneComponent);
		}
	}

}

// Adds random points to the spline of landscape splines
void ASplineTest::GenerateForLandscape()
{
	ALandscape* LocalLandscape = Landscape.Get();
	if (!IsValid(LocalLandscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* SplinesComponent = LocalLandscape->GetSplinesComponent();
	if (!IsValid(SplinesComponent))
	{
		LocalLandscape->CreateSplineComponent();
		SplinesComponent = LocalLandscape->GetSplinesComponent();
	}

	SplinesComponent->Modify();

	for (int32 i = 0; i < 10; i++)
	{
		FVector Location = FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), FMath::RandRange(200, 200));

		ULandscapeSplineControlPoint* NewControlPoint = NewObject<ULandscapeSplineControlPoint>(SplinesComponent, NAME_None, RF_Transactional);
		TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = SplinesComponent->GetControlPoints();

		NewControlPoint->Location = Location;

		int32 ControlPointCount = ControlPoints.Num();

		ControlPoints.Add(NewControlPoint);

		NewControlPoint->UpdateSplinePoints();
	}

	LocalLandscape->RequestSplineLayerUpdate();
}

void ASplineTest::GenerateViaActor()
{
	ALandscapeSplineActor* Actor = SplineActor.Get();
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeSplineActor not found"));
		return;
	}

	ULandscapeInfo* LandscapeInfo = Actor->GetLandscapeInfo();
	ALandscape* LocalLandscape = LandscapeInfo->LandscapeActor.Get();

	if (!IsValid(LocalLandscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* SplinesComponent = LocalLandscape->GetSplinesComponent();
	if (!IsValid(SplinesComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplinesComponent not found"));
		return;
	}

	SplinesComponent->Modify();
	TArray<ULandscapeSplineControlPoint*> CreatedPoints;
	
	for (int32 i = 0; i < 10; i++)
	{
		FVector Location = FVector(FMath::RandRange(-2000, 2000), FMath::RandRange(-2000, 2000), FMath::RandRange(200, 200));

		ULandscapeSplineControlPoint* NewControlPoint = NewObject<ULandscapeSplineControlPoint>(SplinesComponent, NAME_None, RF_Transactional);

		NewControlPoint->Location = Location;
		NewControlPoint->Width = 500.0f; // whatever you need
		NewControlPoint->SideFalloff = 200.0f;
		NewControlPoint->EndFalloff = 200.0f;

		NewControlPoint->Modify();
		SplinesComponent->GetControlPoints().Add(NewControlPoint);

		CreatedPoints.Add(NewControlPoint);
	}

	for (int32 i = 0; i + 1 < CreatedPoints.Num(); ++i)
	{
		ULandscapeSplineControlPoint* Start = CreatedPoints[i];
		ULandscapeSplineControlPoint* End = CreatedPoints[i + 1];

		ULandscapeSplineSegment* NewSegment = NewObject<ULandscapeSplineSegment>(SplinesComponent, NAME_None, RF_Transactional);

		NewSegment->Connections[0].ControlPoint = Start;
		NewSegment->Connections[0].SocketName = NAME_None;

		NewSegment->Connections[1].ControlPoint = End;
		NewSegment->Connections[1].SocketName = NAME_None;

		NewSegment->Modify();
		SplinesComponent->GetSegments().Add(NewSegment);

		// Hook segment into control points
		Start->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 0));
		End->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 1));

		NewSegment->UpdateSplinePoints();
	}

	for (ULandscapeSplineControlPoint* CP : CreatedPoints)
	{
		CP->UpdateSplinePoints();
	}

	SplinesComponent->MarkRenderStateDirty();
	SplinesComponent->MarkPackageDirty();

	LocalLandscape->RequestSplineLayerUpdate();
}

void ASplineTest::GenerateSpline()
{
	ALandscapeSplineActor* Actor = SplineActor.Get();
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeSplineActor not found"));
		return;
	}

	ALandscape* LocalLandscape = Actor->GetLandscapeInfo()->LandscapeActor.Get();
	if (!IsValid(LocalLandscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* LSC = Actor->GetSplinesComponent();
	if (!IsValid(LSC))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplinesComponent not found"));
		return;
	}

	/*
	SplineComponent->ClearSplinePoints(true);


	SplinesComponent->Modify();

	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& LandscapeSplinePoints = SplinesComponent->GetControlPoints();
	//LandscapeSplinePoints.Empty();
	LandscapeSplinePoints.RemoveAt(0);

	TArray<TObjectPtr<ULandscapeSplineSegment>>& LandscapeSegments = SplinesComponent->GetSegments();
	LandscapeSegments.RemoveAt(0);

	//for (int i = 0; i < LandscapeSplinePoints.Num(); i++)
	//{
		//ULandscapeSplineControlPoint* LandscapePoint = LandscapeSplinePoints[i];
		//LandscapePoint->Location.Z += 100.0f;

		//LandscapePoint->UpdateSplinePoints();

		//FVector Point_Worldspace = LandscapePoint->Location + GetActorLocation();

		//SplineComponent->AddSplinePoint(Point_Worldspace, ESplineCoordinateSpace::World, true);
	//}

	SplinesComponent->MarkRenderStateDirty();

	LocalLandscape->RequestSplineLayerUpdate();
	LocalLandscape->RequestLayersContentUpdateForceAll();
	*/


	LSC->Modify();

	int NumPoints = SplineComponent->GetNumberOfSplinePoints();

	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& LandscapeSplinePoints = LSC->GetControlPoints();
	LandscapeSplinePoints.Empty();

	TArray<TObjectPtr<ULandscapeSplineSegment>>& LandscapeSegments = LSC->GetSegments();
	LandscapeSegments.Empty();

	for (int i = 0; i < NumPoints; i++)
	{

		TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = LSC->GetControlPoints();
		int32 CPCount = ControlPoints.Num();

		ULandscapeSplineControlPoint* CP = NewObject<ULandscapeSplineControlPoint>(LSC, NAME_None, RF_Transactional);
		LandscapeSplinePoints.Add(CP);

		CP->Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		CP->Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		CP->Width = 200.0f;
		CP->SideFalloff = 200.0f;
		CP->EndFalloff = 200.0f;

		if (CPCount > 0)
		{
			ULandscapeSplineControlPoint* PreviousPoint = ControlPoints[CPCount - 1];
			AddSegment(PreviousPoint, CP, true, true);
		}

		CP->Modify();
		CP->UpdateSplinePoints();
	}

	// Update all CPs again after connections (optional but safe)
	for (ULandscapeSplineControlPoint* CP : LandscapeSplinePoints)
	{
		CP->UpdateSplinePoints();
	}

	LSC->MarkRenderStateDirty();

	LocalLandscape->RequestSplineLayerUpdate();
	LocalLandscape->RequestLayersContentUpdateForceAll();

}


void ASplineTest::AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd)
{
	if (Start == End)
	{
		//UE_LOG( TEXT("Can't join spline control point to itself.") );
		return;
	}

	if (Start->GetOuterULandscapeSplinesComponent() != End->GetOuterULandscapeSplinesComponent())
	{
		//UE_LOG( TEXT("Can't join spline control points across different terrains.") );
		return;
	}

	for (const FLandscapeSplineConnection& Connection : Start->ConnectedSegments)
	{
		// if the *other* end on the connected segment connects to the "end" control point...
		if (Connection.GetFarConnection().ControlPoint == End)
		{
			//UE_LOG( TEXT("Spline control points already joined connected!") );
			return;
		}
	}

	ULandscapeSplinesComponent* SplinesComponent = Start->GetOuterULandscapeSplinesComponent();
	SplinesComponent->Modify();
	Start->Modify();
	End->Modify();

	ULandscapeSplineSegment* NewSegment = NewObject<ULandscapeSplineSegment>(SplinesComponent, NAME_None, RF_Transactional);
	TArray<TObjectPtr<ULandscapeSplineSegment>>& Segments = SplinesComponent->GetSegments();
	Segments.Add(NewSegment);

	NewSegment->Connections[0].ControlPoint = Start;
	NewSegment->Connections[1].ControlPoint = End;

	NewSegment->Connections[0].SocketName = Start->GetBestConnectionTo(End->Location);
	NewSegment->Connections[1].SocketName = End->GetBestConnectionTo(Start->Location);

	FVector StartLocation; FRotator StartRotation;
	Start->GetConnectionLocationAndRotation(NewSegment->Connections[0].SocketName, StartLocation, StartRotation);
	FVector EndLocation; FRotator EndRotation;
	End->GetConnectionLocationAndRotation(NewSegment->Connections[1].SocketName, EndLocation, EndRotation);

	// Set up tangent lengths
	NewSegment->Connections[0].TangentLen = static_cast<float>((EndLocation - StartLocation).Size());
	NewSegment->Connections[1].TangentLen = NewSegment->Connections[0].TangentLen;

	NewSegment->AutoFlipTangents();

	// set up other segment options
	ULandscapeSplineSegment* CopyFromSegment = nullptr;
	if (Start->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = Start->ConnectedSegments[0].Segment;
	}
	else if (End->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = End->ConnectedSegments[0].Segment;
	}
	else
	{
		// Use defaults
	}

	if (CopyFromSegment != nullptr)
	{
		NewSegment->LayerName = CopyFromSegment->LayerName;
		NewSegment->SplineMeshes = CopyFromSegment->SplineMeshes;
		NewSegment->LDMaxDrawDistance = CopyFromSegment->LDMaxDrawDistance;
		NewSegment->bRaiseTerrain = CopyFromSegment->bRaiseTerrain;
		NewSegment->bLowerTerrain = CopyFromSegment->bLowerTerrain;
		NewSegment->bPlaceSplineMeshesInStreamingLevels = CopyFromSegment->bPlaceSplineMeshesInStreamingLevels;
		NewSegment->BodyInstance = CopyFromSegment->BodyInstance;
		NewSegment->bCastShadow = CopyFromSegment->bCastShadow;
		NewSegment->TranslucencySortPriority = CopyFromSegment->TranslucencySortPriority;
		NewSegment->RuntimeVirtualTextures = CopyFromSegment->RuntimeVirtualTextures;
		NewSegment->VirtualTextureLodBias = CopyFromSegment->VirtualTextureLodBias;
		NewSegment->VirtualTextureCullMips = CopyFromSegment->VirtualTextureCullMips;
		NewSegment->VirtualTextureRenderPassType = CopyFromSegment->VirtualTextureRenderPassType;
		NewSegment->bRenderCustomDepth = CopyFromSegment->bRenderCustomDepth;
		NewSegment->CustomDepthStencilWriteMask = CopyFromSegment->CustomDepthStencilWriteMask;
		NewSegment->CustomDepthStencilValue = CopyFromSegment->CustomDepthStencilValue;
	}

	Start->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 0));
	End->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 1));

	bool bUpdatedStart = false;
	bool bUpdatedEnd = false;
	if (bAutoRotateStart)
	{
		Start->AutoCalcRotation(true);
		Start->UpdateSplinePoints();
		bUpdatedStart = true;
	}
	if (bAutoRotateEnd)
	{
		End->AutoCalcRotation(true);
		End->UpdateSplinePoints();
		bUpdatedEnd = true;
	}

	// Control points' points are currently based on connected segments, so need to be updated.
	if (!bUpdatedStart && Start->Mesh)
	{
		Start->UpdateSplinePoints();
	}
	if (!bUpdatedEnd && End->Mesh)
	{
		End->UpdateSplinePoints();
	}

	// If we've called UpdateSplinePoints on either control point it will already have called UpdateSplinePoints on the new segment
	if (!(bUpdatedStart || bUpdatedEnd))
	{
		NewSegment->UpdateSplinePoints();
	}
}

void ASplineTest::GenerateEditorSpline()
{
	ALandscapeSplineActor* Actor = SplineActor.Get();
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeSplineActor not found"));
		return;
	}

	ALandscape* LocalLandscape = Actor->GetLandscapeInfo()->LandscapeActor.Get();
	if (!IsValid(LocalLandscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* LSC = Actor->GetSplinesComponent();
	if (!IsValid(LSC))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplinesComponent not found"));
		return;
	}

	SplineComponent->ClearSplinePoints(true);

	const TArray<TObjectPtr<ULandscapeSplineControlPoint>>& LandscapeSplinePoints = LSC->GetControlPoints();

	for (int i = 0; i < LandscapeSplinePoints.Num(); i++)
	{
		SplineComponent->AddSplinePoint(LandscapeSplinePoints[i]->Location, ESplineCoordinateSpace::Local);

		SplineComponent->SetRotationAtSplinePoint(i, LandscapeSplinePoints[i]->Rotation, ESplineCoordinateSpace::Local);

		FVector Tangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

		SplineComponent->SetTangentAtSplinePoint(i, (Tangent * 500), ESplineCoordinateSpace::Local);
	}

}

void ASplineTest::GenerateLandscapeSpline()
{
	ALandscapeSplineActor* Actor = SplineActor.Get();
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeSplineActor not found"));
		return;
	}

	ALandscape* LocalLandscape = Actor->GetLandscapeInfo()->LandscapeActor.Get();
	if (!IsValid(LocalLandscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* LSC = Actor->GetSplinesComponent();
	if (!IsValid(LSC))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplinesComponent not found"));
		return;
	}

	/*
	SplineComponent->ClearSplinePoints(true);


	SplinesComponent->Modify();

	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& LandscapeSplinePoints = SplinesComponent->GetControlPoints();
	//LandscapeSplinePoints.Empty();
	LandscapeSplinePoints.RemoveAt(0);

	TArray<TObjectPtr<ULandscapeSplineSegment>>& LandscapeSegments = SplinesComponent->GetSegments();
	LandscapeSegments.RemoveAt(0);

	//for (int i = 0; i < LandscapeSplinePoints.Num(); i++)
	//{
		//ULandscapeSplineControlPoint* LandscapePoint = LandscapeSplinePoints[i];
		//LandscapePoint->Location.Z += 100.0f;

		//LandscapePoint->UpdateSplinePoints();

		//FVector Point_Worldspace = LandscapePoint->Location + GetActorLocation();

		//SplineComponent->AddSplinePoint(Point_Worldspace, ESplineCoordinateSpace::World, true);
	//}

	SplinesComponent->MarkRenderStateDirty();

	LocalLandscape->RequestSplineLayerUpdate();
	LocalLandscape->RequestLayersContentUpdateForceAll();
	*/


	LSC->Modify();

	int NumPoints = SplineComponent->GetNumberOfSplinePoints();

	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& LandscapeSplinePoints = LSC->GetControlPoints();
	LandscapeSplinePoints.Empty();

	TArray<TObjectPtr<ULandscapeSplineSegment>>& LandscapeSegments = LSC->GetSegments();
	LandscapeSegments.Empty();

	for (int i = 0; i < NumPoints; i++)
	{

		TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = LSC->GetControlPoints();
		int32 CPCount = ControlPoints.Num();

		ULandscapeSplineControlPoint* CP = NewObject<ULandscapeSplineControlPoint>(LSC, NAME_None, RF_Transactional);
		LandscapeSplinePoints.Add(CP);

		CP->Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		CP->Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		CP->Width = 200.0f;
		CP->SideFalloff = 200.0f;
		CP->EndFalloff = 200.0f;

		if (CPCount > 0)
		{
			ULandscapeSplineControlPoint* PreviousPoint = ControlPoints[CPCount - 1];
			AddSegment(PreviousPoint, CP, true, true);
		}

		CP->Modify();
		CP->UpdateSplinePoints();
	}

	// Update all CPs again after connections (optional but safe)
	for (ULandscapeSplineControlPoint* CP : LandscapeSplinePoints)
	{
		CP->UpdateSplinePoints();
	}

	LSC->MarkRenderStateDirty();

	LocalLandscape->RequestSplineLayerUpdate();
	LocalLandscape->RequestLayersContentUpdateForceAll();
}






ASplineSegment::ASplineSegment()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	if (IsValid(SceneComponent))
	{
		SetRootComponent(SceneComponent);

#if WITH_EDITOR

		SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
		if (IsValid(SplineComponent))
		{
			SplineComponent->SetupAttachment(SceneComponent);
			SplineComponent->bDrawDebug = true;
			SplineComponent->bShouldVisualizeScale = true;
			SplineComponent->EditorUnselectedSplineSegmentColor = FColor::FromHex("#0026FFFF");
			SplineComponent->EditorSelectedSplineSegmentColor = FColor::FromHex("#0026FFFF");
			SplineComponent->EditorTangentColor = FColor::FromHex("#FF0056FF");
		}

#endif

		StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		if (IsValid(StaticMeshComponent))
		{
			StaticMeshComponent->SetupAttachment(SceneComponent);
		}
	}

#if WITH_EDITOR

	ExportPath.Path = TEXT("/Game");

#endif
}

#if WITH_EDITOR

void ASplineSegment::BuildSpline()
{
	if (StartPoint.bEnabled)
	{
		AActor* Actor = StartPoint.Actor.Get();
		if (!IsValid(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor not found"));
			return;
		}

		if (StartPoint.SegmentType == ESegmentType::Socket)
		{
			UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
			if (!IsValid(MeshComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found"));
				return;
			}

			bool bSocketExists = MeshComponent->DoesSocketExist(StartPoint.SocketName);
			if (!bSocketExists)
			{
				UE_LOG(LogTemp, Warning, TEXT("Socket not found"));
				return;
			}

			FVector SocketLocation = MeshComponent->GetSocketLocation(StartPoint.SocketName);
			FRotator SocketRotation = MeshComponent->GetSocketRotation(StartPoint.SocketName);

			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(SocketRotation);
			FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX(ForwardVector);

			FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, SocketRotation.Roll);

			SplineComponent->SetLocationAtSplinePoint(0, SocketLocation, ESplineCoordinateSpace::World);
			SplineComponent->SetRotationAtSplinePoint(0, NewRotator, ESplineCoordinateSpace::World);

			FVector Tangent = SplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
			FVector NewTangent = Tangent * FMath::Max(0.1f, StartPoint.TangentMultiplier);

			SplineComponent->SetTangentAtSplinePoint(0, NewTangent, ESplineCoordinateSpace::World);
		}
		else if (StartPoint.SegmentType == ESegmentType::Spline)
		{
			USplineComponent* Spline = Actor->FindComponentByClass<USplineComponent>();
			if (!IsValid(Spline))
			{
				UE_LOG(LogTemp, Warning, TEXT("SplineComponent not found"));
				return;
			}

			FVector PointLocation;
			FRotator PointRotation;
			FVector PointTangent;

			bool bSuccess = GetSplineData(Spline, StartPoint.PointPosition, PointLocation, PointRotation, PointTangent);
			if (!bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get spline data"));
				return;
			}

			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(PointRotation);
			FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX(ForwardVector);

			if (StartPoint.PointPosition == ESplinePositionType::Start)
			{
				FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (ForwardRotator.Roll * -1.0f));

				SplineComponent->SetLocationAtSplinePoint(0, PointLocation, ESplineCoordinateSpace::World);
				SplineComponent->SetRotationAtSplinePoint(0, NewRotator, ESplineCoordinateSpace::World);
				SplineComponent->SetTangentAtSplinePoint(0, (PointTangent * -1.0f), ESplineCoordinateSpace::World);
			}
			else
			{
				FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, ForwardRotator.Roll);

				SplineComponent->SetLocationAtSplinePoint(0, PointLocation, ESplineCoordinateSpace::World);
				SplineComponent->SetRotationAtSplinePoint(0, NewRotator, ESplineCoordinateSpace::World);
				SplineComponent->SetTangentAtSplinePoint(0, PointTangent, ESplineCoordinateSpace::World);
			}
		}
	}

	
	if (EndPoint.bEnabled)
	{
		AActor* Actor = EndPoint.Actor.Get();
		if (!IsValid(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor not found"));
			return;
		}

		if (EndPoint.SegmentType == ESegmentType::Socket)
		{
			UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
			if (!IsValid(MeshComponent))
			{
				UE_LOG(LogTemp, Warning, TEXT("MeshComponent not found"));
				return;
			}

			bool bSocketExists = MeshComponent->DoesSocketExist(EndPoint.SocketName);
			if (!bSocketExists)
			{
				UE_LOG(LogTemp, Warning, TEXT("Socket not found"));
				return;
			}

			int LastIndex = SplineComponent->GetNumberOfSplinePoints() - 1;

			FVector SocketLocation = MeshComponent->GetSocketLocation(EndPoint.SocketName);
			FRotator SocketRotation = MeshComponent->GetSocketRotation(EndPoint.SocketName);

			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(SocketRotation);
			FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX((ForwardVector * -1.0f));

			FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (SocketRotation.Roll * -1.0f));

			SplineComponent->SetLocationAtSplinePoint(LastIndex, SocketLocation, ESplineCoordinateSpace::World);
			SplineComponent->SetRotationAtSplinePoint(LastIndex, NewRotator, ESplineCoordinateSpace::World);

			FVector Tangent = SplineComponent->GetTangentAtSplinePoint(LastIndex, ESplineCoordinateSpace::World);
			FVector NewTangent = Tangent * FMath::Max(0.1f, EndPoint.TangentMultiplier);

			SplineComponent->SetTangentAtSplinePoint(LastIndex, NewTangent, ESplineCoordinateSpace::World);
		}
		else if (EndPoint.SegmentType == ESegmentType::Spline)
		{
			USplineComponent* Spline = Actor->FindComponentByClass<USplineComponent>();
			if (!IsValid(Spline))
			{
				UE_LOG(LogTemp, Warning, TEXT("SplineComponent not found"));
				return;
			}

			int LastIndex = SplineComponent->GetNumberOfSplinePoints() - 1;

			FVector PointLocation;
			FRotator PointRotation;
			FVector PointTangent;

			bool bSuccess = GetSplineData(Spline, StartPoint.PointPosition, PointLocation, PointRotation, PointTangent);
			if (!bSuccess)
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get spline data"));
				return;
			}

			FVector ForwardVector = UKismetMathLibrary::GetForwardVector(PointRotation);
			FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX(ForwardVector);

			if (StartPoint.PointPosition == ESplinePositionType::Start)
			{
				FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, ForwardRotator.Roll);

				SplineComponent->SetLocationAtSplinePoint(LastIndex, PointLocation, ESplineCoordinateSpace::World);
				SplineComponent->SetRotationAtSplinePoint(LastIndex, NewRotator, ESplineCoordinateSpace::World);
				SplineComponent->SetTangentAtSplinePoint(LastIndex, PointTangent, ESplineCoordinateSpace::World);
			}
			else
			{
				FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (ForwardRotator.Roll * -1.0f));

				SplineComponent->SetLocationAtSplinePoint(LastIndex, PointLocation, ESplineCoordinateSpace::World);
				SplineComponent->SetRotationAtSplinePoint(LastIndex, NewRotator, ESplineCoordinateSpace::World);
				SplineComponent->SetTangentAtSplinePoint(LastIndex, (PointTangent * -1.0f), ESplineCoordinateSpace::World);
			}
		}
	}
}


void ASplineSegment::BuildSplineMeshes()
{
	RemoveSplineMeshes();

	if (!IsValid(SegmentMesh))
	{
		UE_LOG(LogTemp, Warning, TEXT("SegmentMesh is invalid"));
		return;
	}

	int Num = SplineComponent->GetNumberOfSplinePoints() - 1;
	if (Num == 0)
	{
		return;
	}

	for (int i = 0; i < Num; i++)
	{
		USplineMeshComponent* NewMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (IsValid(NewMesh))
		{
			FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			float StartRoll = SplineComponent->GetRollAtSplinePoint(i, ESplineCoordinateSpace::Local);

			FVector NextLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			FVector NextTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			float EndRoll = SplineComponent->GetRollAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			NewMesh->SetStaticMesh(SegmentMesh);
			NewMesh->SetForwardAxis(ESplineMeshAxis::X);

			NewMesh->RegisterComponent();
			AddInstanceComponent(NewMesh);

			NewMesh->SetMobility(EComponentMobility::Movable);
			NewMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			NewMesh->SetStartRollDegrees(StartRoll);
			NewMesh->SetEndRollDegrees(EndRoll);

			NewMesh->SetStartAndEnd(StartLocation, StartTangent, NextLocation, NextTangent);
		}
	}
}

void ASplineSegment::RemoveSplineMeshes()
{
	TArray<USplineMeshComponent*> SplineMeshes;
	const TSet<UActorComponent*>& Components = GetComponents();

	for (UActorComponent* Component : Components)
	{
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(Component);
		if (IsValid(SplineMesh))
		{
			SplineMeshes.Add(SplineMesh);
		}
	}

	for (USplineMeshComponent* SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
	}
}


void ASplineSegment::SnapSplineToSurface(bool bEnableRotation)
{
	UWorld* World = GetWorld();
	int Count = SplineComponent->GetNumberOfSplinePoints();
	int Num = Count - 1;

	TArray<FVector> HitLocations;
	HitLocations.Reserve(Count);

	TArray<float> HitRolls;
	HitRolls.Reserve(Count);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	for (int i = 0; i < Num; i++)
	{
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		StartLocation.Z += 100.0f;

		FVector EndLocation = StartLocation;
		EndLocation.Z -= 1000.0f;

		FHitResult HitResult;

		bool bHit = UKismetSystemLibrary::LineTraceSingle(World, StartLocation, EndLocation, ETraceTypeQuery::TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
		if (bHit)
		{
			HitLocations.Add(HitResult.Location);

			if (bEnableRotation)
			{
				FVector Normal = HitResult.Normal;
				FVector Forward = SplineComponent->GetDirectionAtSplinePoint(i, ESplineCoordinateSpace::World);

				FVector Right = FVector::CrossProduct(Forward, Normal).GetSafeNormal();
				FVector AdjustedUp = FVector::CrossProduct(Right, Forward).GetSafeNormal();

				float RollRadians = FMath::Atan2(
					FVector::DotProduct(Right, FVector::UpVector),
					FVector::DotProduct(AdjustedUp, FVector::UpVector)
				);

				float RollDegrees = FMath::RadiansToDegrees(RollRadians);

				HitRolls.Add(RollDegrees);
			}
		}
		else
		{
			HitLocations.Add(FVector::ZeroVector);

			if (bEnableRotation)
			{
				HitRolls.Add(0.0f);
			}
		}
	}

	for (int i = 0; i < Num; i++)
	{
		FVector Location = HitLocations[i];
		Location.Z += SnapOffset;

		SplineComponent->SetLocationAtSplinePoint(i, Location, ESplineCoordinateSpace::World);

		if (bEnableRotation)
		{
			FVector OldTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
			float TangentLength = OldTangent.Length();

			FRotator Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World);
			Rotation.Roll = HitRolls[i];
			SplineComponent->SetRotationAtSplinePoint(i, Rotation, ESplineCoordinateSpace::World);

			FVector NewTangent = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::World);
			SplineComponent->SetTangentAtSplinePoint(i, (NewTangent * TangentLength), ESplineCoordinateSpace::World);
		}
	}
}


void ASplineSegment::RecenterActor()
{
	if (!SplineComponent)
	{
		return;
	}

	const int Num = SplineComponent->GetNumberOfSplinePoints();
	if (Num == 0)
	{
		return;
	}

	FVector LocationSum = FVector::ZeroVector;

	TArray<FVector> PointLocations;

	for (int i = 0; i < Num; i++)
	{
		FVector Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		PointLocations.Add(Location);

		LocationSum += Location;
	}

	SetActorLocation(LocationSum / Num);

	for (int i = 0; i < Num; i++)
	{
		FVector Location = PointLocations[i];
		SplineComponent->SetLocationAtSplinePoint(i, Location, ESplineCoordinateSpace::World);
	}
}


void ASplineSegment::AddSegment(ULandscapeSplineControlPoint* Start, ULandscapeSplineControlPoint* End, bool bAutoRotateStart, bool bAutoRotateEnd)
{
	if (Start == End)
	{
		//UE_LOG( TEXT("Can't join spline control point to itself.") );
		return;
	}

	if (Start->GetOuterULandscapeSplinesComponent() != End->GetOuterULandscapeSplinesComponent())
	{
		//UE_LOG( TEXT("Can't join spline control points across different terrains.") );
		return;
	}

	for (const FLandscapeSplineConnection& Connection : Start->ConnectedSegments)
	{
		// if the *other* end on the connected segment connects to the "end" control point...
		if (Connection.GetFarConnection().ControlPoint == End)
		{
			//UE_LOG( TEXT("Spline control points already joined connected!") );
			return;
		}
	}

	ULandscapeSplinesComponent* SplinesComponent = Start->GetOuterULandscapeSplinesComponent();
	SplinesComponent->Modify();
	Start->Modify();
	End->Modify();

	ULandscapeSplineSegment* NewSegment = NewObject<ULandscapeSplineSegment>(SplinesComponent, NAME_None, RF_Transactional);
	TArray<TObjectPtr<ULandscapeSplineSegment>>& Segments = SplinesComponent->GetSegments();
	Segments.Add(NewSegment);

	NewSegment->Connections[0].ControlPoint = Start;
	NewSegment->Connections[1].ControlPoint = End;

	NewSegment->Connections[0].SocketName = Start->GetBestConnectionTo(End->Location);
	NewSegment->Connections[1].SocketName = End->GetBestConnectionTo(Start->Location);

	FVector StartLocation; FRotator StartRotation;
	Start->GetConnectionLocationAndRotation(NewSegment->Connections[0].SocketName, StartLocation, StartRotation);

	FVector EndLocation; FRotator EndRotation;
	End->GetConnectionLocationAndRotation(NewSegment->Connections[1].SocketName, EndLocation, EndRotation);

	// Set up tangent lengths
	NewSegment->Connections[0].TangentLen = static_cast<float>((EndLocation - StartLocation).Size());
	NewSegment->Connections[1].TangentLen = NewSegment->Connections[0].TangentLen;

	NewSegment->AutoFlipTangents();

	// set up other segment options
	ULandscapeSplineSegment* CopyFromSegment = nullptr;
	if (Start->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = Start->ConnectedSegments[0].Segment;
	}
	else if (End->ConnectedSegments.Num() > 0)
	{
		CopyFromSegment = End->ConnectedSegments[0].Segment;
	}
	else
	{
		// Use defaults
	}

	if (CopyFromSegment != nullptr)
	{
		NewSegment->LayerName = CopyFromSegment->LayerName;
		NewSegment->SplineMeshes = CopyFromSegment->SplineMeshes;
		NewSegment->LDMaxDrawDistance = CopyFromSegment->LDMaxDrawDistance;
		NewSegment->bRaiseTerrain = CopyFromSegment->bRaiseTerrain;
		NewSegment->bLowerTerrain = CopyFromSegment->bLowerTerrain;
		NewSegment->bPlaceSplineMeshesInStreamingLevels = CopyFromSegment->bPlaceSplineMeshesInStreamingLevels;
		NewSegment->BodyInstance = CopyFromSegment->BodyInstance;
		NewSegment->bCastShadow = CopyFromSegment->bCastShadow;
		NewSegment->TranslucencySortPriority = CopyFromSegment->TranslucencySortPriority;
		NewSegment->RuntimeVirtualTextures = CopyFromSegment->RuntimeVirtualTextures;
		NewSegment->VirtualTextureLodBias = CopyFromSegment->VirtualTextureLodBias;
		NewSegment->VirtualTextureCullMips = CopyFromSegment->VirtualTextureCullMips;
		NewSegment->VirtualTextureRenderPassType = CopyFromSegment->VirtualTextureRenderPassType;
		NewSegment->bRenderCustomDepth = CopyFromSegment->bRenderCustomDepth;
		NewSegment->CustomDepthStencilWriteMask = CopyFromSegment->CustomDepthStencilWriteMask;
		NewSegment->CustomDepthStencilValue = CopyFromSegment->CustomDepthStencilValue;
	}

	Start->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 0));
	End->ConnectedSegments.Add(FLandscapeSplineConnection(NewSegment, 1));

	bool bUpdatedStart = false;
	bool bUpdatedEnd = false;
	if (bAutoRotateStart)
	{
		Start->AutoCalcRotation(true);
		Start->UpdateSplinePoints();
		bUpdatedStart = true;
	}
	if (bAutoRotateEnd)
	{
		End->AutoCalcRotation(true);
		End->UpdateSplinePoints();
		bUpdatedEnd = true;
	}

	// Control points' points are currently based on connected segments, so need to be updated.
	if (!bUpdatedStart && Start->Mesh)
	{
		Start->UpdateSplinePoints();
	}
	if (!bUpdatedEnd && End->Mesh)
	{
		End->UpdateSplinePoints();
	}

	// If we've called UpdateSplinePoints on either control point it will already have called UpdateSplinePoints on the new segment
	if (!(bUpdatedStart || bUpdatedEnd))
	{
		NewSegment->UpdateSplinePoints();
	}
}



void ASplineSegment::BuildLandscapeSpline()
{
	bool bOldAutoUpdate = bAutoUpdate;
	bAutoUpdate = false;

	ALandscapeSplineActor* Actor = LandscapeSpline.Get();
	if (!IsValid(Actor))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeSplineActor not found"));
		return;
	}

	ULandscapeInfo* LandscapeInfo = Actor->GetLandscapeInfo();
	if (!IsValid(LandscapeInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("LandscapeInfo not found"));
		return;
	}

	ALandscape* Landscape = LandscapeInfo->LandscapeActor.Get();
	if (!IsValid(Landscape))
	{
		UE_LOG(LogTemp, Warning, TEXT("Landscape not found"));
		return;
	}

	ULandscapeSplinesComponent* SplinesComponent = Actor->GetSplinesComponent();
	if (!IsValid(SplinesComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplinesComponent not found"));
		return;
	}

	Actor->SetActorLocation(GetActorLocation());

	SplinesComponent->Modify();


	TArray<TObjectPtr<ULandscapeSplineControlPoint>>& SplineControlPoints = SplinesComponent->GetControlPoints();
	SplineControlPoints.Empty();

	TArray<TObjectPtr<ULandscapeSplineSegment>>& LandscapeSegments = SplinesComponent->GetSegments();
	LandscapeSegments.Empty();


	int NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();

	for (int i = 0; i < NumSplinePoints; i++)
	{
		TArray<TObjectPtr<ULandscapeSplineControlPoint>>& ControlPoints = SplinesComponent->GetControlPoints();
		int NumControlPoints = ControlPoints.Num();

		ULandscapeSplineControlPoint* NewControlPoint = NewObject<ULandscapeSplineControlPoint>(SplinesComponent, NAME_None, RF_Transactional);
		if (!NewControlPoint)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create LandscapeSplineControlPoint"));
			return;
		}

		SplineControlPoints.Add(NewControlPoint);

		NewControlPoint->Location = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		NewControlPoint->Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		NewControlPoint->Width = 200.0f;
		NewControlPoint->SideFalloff = 200.0f;
		NewControlPoint->EndFalloff = 200.0f;

		if (NumControlPoints > 0)
		{
			ULandscapeSplineControlPoint* PreviousPoint = ControlPoints[NumControlPoints - 1];
			AddSegment(PreviousPoint, NewControlPoint, true, true);
		}

		NewControlPoint->Modify();
		NewControlPoint->UpdateSplinePoints();
	}

	for (ULandscapeSplineControlPoint* ControlPoint : SplineControlPoints)
	{
		ControlPoint->UpdateSplinePoints();
	}

	SplinesComponent->MarkRenderStateDirty();

	Landscape->RequestSplineLayerUpdate();
	Landscape->RequestLayersContentUpdateForceAll();

	// bAutoUpdate = bOldAutoUpdate;
}

void ASplineSegment::BakeSplineMesh()
{
	TArray<UPrimitiveComponent*> MeshComponents;

	const TSet<UActorComponent*>& Components = GetComponents();
	for (UActorComponent* Component : Components)
	{
		if (USplineMeshComponent* Mesh = Cast<USplineMeshComponent>(Component))
		{
			MeshComponents.Add(Mesh);
		}
	}

	if (MeshComponents.Num() == 0 || ExportName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("MeshComponents, ExportName is empty"));
		return;
	}

	const IMeshMergeUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshMergeModule>("MeshMergeUtilities").GetUtilities();

	FVector Location = GetActorLocation();

	FString LocationX = TEXT("x") + FString::FromInt(Location.X).Replace(TEXT("-"), TEXT("n"));
	FString LocationY = TEXT("y") + FString::FromInt(Location.Y).Replace(TEXT("-"), TEXT("n"));

	FString PackageName = TEXT("/Game/") + ExportName + TEXT("_") + LocationX + TEXT("_") + LocationY;
	FVector MergeLocation = FVector::ZeroVector;

	constexpr float ScreenSize = TNumericLimits<float>::Max();

	UWorld* World = GetWorld();

	FMeshMergingSettings Settings;
	Settings.bGenerateLightMapUV = false;

	UE_LOG(LogTemp, Error, TEXT("Baked Package: %s"), *PackageName);

	TArray<UObject*> MergedAssets;
	MeshUtilities.MergeComponentsToStaticMesh(MeshComponents, World, Settings, nullptr, nullptr, PackageName, MergedAssets, MergeLocation, ScreenSize, true);

	if (MergedAssets.IsValidIndex(0))
	{
		UObject* NewAsset = MergedAssets[0];
		UStaticMesh* NewMesh = Cast<UStaticMesh>(NewAsset);
		if (!IsValid(NewMesh))
		{
			UE_LOG(LogTemp, Warning, TEXT("New StaticMesh is invalid"));
			return;
		}

		StaticMeshComponent->SetStaticMesh(NewMesh);

		bAutoUpdate = false;

		if (bClearSplineMeshAfterBuild)
		{
			RemoveSplineMeshes();
		}

		MarkPackageDirty();
	}

	FEditorFileUtils::SaveDirtyPackages(true, true, true, false);
}

bool ASplineSegment::GetSplineData(USplineComponent* InSpline, ESplinePositionType InPointPosition, FVector& OutLocation, FRotator& OutRotation, FVector& OutTangent)
{
	if (!IsValid(InSpline))
	{
		UE_LOG(LogTemp, Warning, TEXT("SplineComponent is invalid"));
		return false;
	}

	int Index = 0;
	if (InPointPosition == ESplinePositionType::End)
	{
		Index = InSpline->GetNumberOfSplinePoints() - 1;
	}
	
	OutLocation = InSpline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	OutRotation = InSpline->GetRotationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	OutTangent = InSpline->GetTangentAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}

void ASplineSegment::OnConstruction(const FTransform& Transform)
{
	if (bAutoUpdate)
	{
		if (bEnableSnap)
		{
			SnapSplineToSurface(bEnableSnapRotation);
		}

		BuildSpline();

		if (bEnableMeshBuild)
		{
			BuildSplineMeshes();
		}
	}
}

#endif

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
#include "EngineUtils.h"
#include "Landscape.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineControlPoint.h"
#include "LandscapeSplineSegment.h"
#include "LandscapeSplineActor.h"

// Project Header
#include "RenEnvironment/Public/Component/OrbitalLightComponent.h"



AEnvironmentActor::AEnvironmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(TEXT("Actor.Environment"));
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	if(IsValid(SceneComponent))
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

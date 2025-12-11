// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/SplineSegment.h"

// Engine Headers
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"

#if WITH_EDITOR

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "Landscape.h"
#include "LandscapeSplinesComponent.h"
#include "LandscapeSplineControlPoint.h"
#include "LandscapeSplineSegment.h"
#include "LandscapeSplineActor.h"

#include "IMeshMergeUtilities.h"
#include "MeshMergeModule.h"
#include "FileHelpers.h"

#endif

// Project Header
#include "RCoreLibrary/Public/LogCategory.h"
#include "RCoreLibrary/Public/LogMacro.h"



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
	int NumPoints = SplineComponent->GetNumberOfSplinePoints();
	bool bHasStart = NumPoints >= 1;
	bool bHasEnd = NumPoints >= 2;

	if (StartPoint.bEnabled && bHasStart)
	{
		if (StartPoint.SegmentType == ESegmentType::Socket)
		{
			JoinSplineToSocket(StartPoint, 0, 1.0f);
		}
		else if (StartPoint.SegmentType == ESegmentType::Spline)
		{
			JoinSplineToSpline(StartPoint, 0, -1.0f, 1.0f);
		}
	}

	if (EndPoint.bEnabled && bHasEnd)
	{
		int LastIndex = NumPoints - 1;

		if (EndPoint.SegmentType == ESegmentType::Socket)
		{
			JoinSplineToSocket(EndPoint, LastIndex, -1.0f);
		}
		else if (EndPoint.SegmentType == ESegmentType::Spline)
		{
			JoinSplineToSpline(EndPoint, LastIndex, 1.0f, -1.0f);
		}
	}
}

void ASplineSegment::JoinSplineToSocket(const FSegmentData& SegmentData, int Index, float Direction)
{
	AActor* Actor = SegmentData.Actor.Get();
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid"));
		return;
	}

	UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
	if (!IsValid(MeshComponent))
	{
		LOG_ERROR(LogEnvironment, TEXT("MeshComponent is invalid"));
		return;
	}

	bool bSocketExists = MeshComponent->DoesSocketExist(SegmentData.SocketName);
	if (!bSocketExists)
	{
		LOG_ERROR(LogEnvironment, TEXT("Socket does not exist"));
		return;
	}

	FVector SocketLocation = MeshComponent->GetSocketLocation(SegmentData.SocketName);
	FRotator SocketRotation = MeshComponent->GetSocketRotation(SegmentData.SocketName);

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(SocketRotation);
	FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX((ForwardVector * Direction));

	FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (SocketRotation.Roll * Direction));

	SplineComponent->SetLocationAtSplinePoint(Index, SocketLocation, ESplineCoordinateSpace::World);
	SplineComponent->SetRotationAtSplinePoint(Index, NewRotator, ESplineCoordinateSpace::World);

	FVector Tangent = SplineComponent->GetTangentAtSplinePoint(Index, ESplineCoordinateSpace::World);
	FVector NewTangent = Tangent * FMath::Max(0.1f, SegmentData.TangentSize);

	SplineComponent->SetTangentAtSplinePoint(Index, NewTangent, ESplineCoordinateSpace::World);
}

void ASplineSegment::JoinSplineToSpline(const FSegmentData& SegmentData, int Index, float StartDirection, float EndDirection)
{
	AActor* Actor = SegmentData.Actor.Get();
	if (!IsValid(Actor))
	{
		LOG_ERROR(LogEnvironment, TEXT("Actor is invalid"));
		return;
	}

	USplineComponent* Spline = Actor->FindComponentByClass<USplineComponent>();
	if (!IsValid(Spline))
	{
		LOG_ERROR(LogEnvironment, TEXT("Spline is invalid"));
		return;
	}

	FVector PointLocation;
	FRotator PointRotation;
	FVector PointTangent;

	bool bSuccess = GetSplineData(Spline, SegmentData.SplinePosition, PointLocation, PointRotation, PointTangent);
	if (!bSuccess)
	{
		LOG_ERROR(LogEnvironment, TEXT("Failed to get spline data"));
		return;
	}

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(PointRotation);
	FRotator ForwardRotator = UKismetMathLibrary::MakeRotFromX(ForwardVector);

	if (SegmentData.SplinePosition == ESegmentSplinePosition::Start)
	{
		FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (ForwardRotator.Roll * StartDirection));

		SplineComponent->SetLocationAtSplinePoint(Index, PointLocation, ESplineCoordinateSpace::World);
		SplineComponent->SetRotationAtSplinePoint(Index, NewRotator, ESplineCoordinateSpace::World);
		SplineComponent->SetTangentAtSplinePoint(Index, (PointTangent * StartDirection), ESplineCoordinateSpace::World);
	}
	else
	{
		FRotator NewRotator = FRotator(ForwardRotator.Pitch, ForwardRotator.Yaw, (ForwardRotator.Roll * EndDirection));

		SplineComponent->SetLocationAtSplinePoint(Index, PointLocation, ESplineCoordinateSpace::World);
		SplineComponent->SetRotationAtSplinePoint(Index, NewRotator, ESplineCoordinateSpace::World);
		SplineComponent->SetTangentAtSplinePoint(Index, (PointTangent * EndDirection), ESplineCoordinateSpace::World);
	}
}

bool ASplineSegment::GetSplineData(USplineComponent* InSpline, ESegmentSplinePosition InSplinePosition, FVector& OutLocation, FRotator& OutRotation, FVector& OutTangent)
{
	if (!IsValid(InSpline))
	{
		LOG_ERROR(LogEnvironment, TEXT("InSpline is invalid"));
		return false;
	}

	int Index = 0;
	if (InSplinePosition == ESegmentSplinePosition::End)
	{
		Index = InSpline->GetNumberOfSplinePoints() - 1;
	}

	OutLocation = InSpline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	OutRotation = InSpline->GetRotationAtSplinePoint(Index, ESplineCoordinateSpace::World);
	OutTangent = InSpline->GetTangentAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}



void ASplineSegment::BuildSplineMeshByPoints()
{
	RemoveSplineMeshes();

	if (!IsValid(SegmentMesh))
	{
		LOG_ERROR(LogEnvironment, TEXT("SegmentMesh is invalid"));
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
			FVector StartRight = SplineComponent->GetRightVectorAtSplinePoint(i, ESplineCoordinateSpace::Local);
			float StartRoll = SplineComponent->GetRollAtSplinePoint(i, ESplineCoordinateSpace::Local);

			FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			FVector EndRight = SplineComponent->GetRightVectorAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
			float EndRoll = SplineComponent->GetRollAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			StartLocation += StartRight * SideOffset;
			EndLocation += EndRight * SideOffset;

			NewMesh->SetStaticMesh(SegmentMesh);
			NewMesh->SetForwardAxis(ESplineMeshAxis::X);
			NewMesh->SetCollisionProfileName(TEXT("BlockAll"));

			NewMesh->RegisterComponent();
			AddInstanceComponent(NewMesh);

			NewMesh->SetMobility(EComponentMobility::Movable);
			NewMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			NewMesh->SetStartRollDegrees(StartRoll);
			NewMesh->SetEndRollDegrees(EndRoll);

			NewMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
		}
	}
}

void ASplineSegment::BuildSplineMeshByDistance()
{
	RemoveSplineMeshes();

	if (!IsValid(SegmentMesh))
	{
		LOG_ERROR(LogEnvironment, TEXT("SegmentMesh is invalid"));
		return;
	}

	float SplineLength = SplineComponent->GetSplineLength();
	int SegmentCount = FMath::TruncToInt(SplineLength / SegmentSize);

	for (int i = 0; i < SegmentCount; i++)
	{
		USplineMeshComponent* NewMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		if (IsValid(NewMesh))
		{
			float StartDistance = i * SegmentSize;
			FVector StartLocation = SplineComponent->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			FVector StartRight = SplineComponent->GetRightVectorAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			float StartRoll = SplineComponent->GetRollAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);

			float EndDistance = (i + 1) * SegmentSize;
			FVector EndLocation = SplineComponent->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
			FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
			FVector EndRight = SplineComponent->GetRightVectorAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
			float EndRoll = SplineComponent->GetRollAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);

			StartLocation += StartRight * SideOffset;
			EndLocation += EndRight * SideOffset;

			NewMesh->SetStaticMesh(SegmentMesh);
			NewMesh->SetForwardAxis(ESplineMeshAxis::X);
			NewMesh->SetCollisionProfileName(TEXT("BlockAll"));

			NewMesh->RegisterComponent();
			AddInstanceComponent(NewMesh);

			NewMesh->SetMobility(EComponentMobility::Movable);
			NewMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			NewMesh->SetStartRollDegrees(StartRoll);
			NewMesh->SetEndRollDegrees(EndRoll);

			FVector ClampedStartTangent = UKismetMathLibrary::ClampVectorSize(StartTangent, 1.0f, SegmentSize);
			FVector ClampedEndTangent = UKismetMathLibrary::ClampVectorSize(EndTangent, 1.0f, SegmentSize);

			NewMesh->SetStartAndEnd(StartLocation, ClampedStartTangent, EndLocation, ClampedEndTangent);
		}
	}
}


void ASplineSegment::BuildSplineMeshes()
{
	if (bBuildMeshByPoints)
	{
		BuildSplineMeshByPoints();
	}
	else
	{
		BuildSplineMeshByDistance();
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
		SplineMesh->MarkAsGarbage();
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
		LOG_ERROR(LogEnvironment, TEXT("Can't join spline control point to itself"));
		return;
	}

	if (Start->GetOuterULandscapeSplinesComponent() != End->GetOuterULandscapeSplinesComponent())
	{
		LOG_ERROR(LogEnvironment, TEXT("Can't join spline control points across different terrains"));
		return;
	}

	for (const FLandscapeSplineConnection& Connection : Start->ConnectedSegments)
	{
		if (Connection.GetFarConnection().ControlPoint == End)
		{
			LOG_ERROR(LogEnvironment, TEXT("Spline control points already joined connected!"));
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
		LOG_ERROR(LogEnvironment, TEXT("LandscapeSplineActor is invalid"));
		return;
	}

	ULandscapeInfo* LandscapeInfo = Actor->GetLandscapeInfo();
	if (!IsValid(LandscapeInfo))
	{
		LOG_ERROR(LogEnvironment, TEXT("LandscapeInfo is invalid"));
		return;
	}

	ALandscape* Landscape = LandscapeInfo->LandscapeActor.Get();
	if (!IsValid(Landscape))
	{
		LOG_ERROR(LogEnvironment, TEXT("Landscape is invalid"));
		return;
	}

	ULandscapeSplinesComponent* SplinesComponent = Actor->GetSplinesComponent();
	if (!IsValid(SplinesComponent))
	{
		LOG_ERROR(LogEnvironment, TEXT("SplinesComponent is invalid"));
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
			LOG_ERROR(LogEnvironment, TEXT("Failed to create LandscapeSplineControlPoint"));
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
	bAutoUpdate = false;

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
		LOG_ERROR(LogEnvironment, TEXT("MeshComponents, ExportName is empty"));
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
	Settings.bAllowDistanceField = true;
	Settings.bMergePhysicsData = true;

	LOG_INFO(LogEnvironment, TEXT("Baked Package: %s"), *PackageName);

	TArray<UObject*> MergedAssets;
	MeshUtilities.MergeComponentsToStaticMesh(MeshComponents, World, Settings, nullptr, nullptr, PackageName, MergedAssets, MergeLocation, ScreenSize, true);

	if (MergedAssets.IsValidIndex(0))
	{
		UObject* NewAsset = MergedAssets[0];
		UStaticMesh* NewMesh = Cast<UStaticMesh>(NewAsset);
		if (!IsValid(NewMesh))
		{
			LOG_ERROR(LogEnvironment, TEXT("NewMesh is invalid"));
			return;
		}

		StaticMeshComponent->SetStaticMesh(NewMesh);

		if (bClearMeshAfterBuild)
		{
			RemoveSplineMeshes();
		}
	}

	FEditorFileUtils::SaveDirtyPackages(true, true, true, false);
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


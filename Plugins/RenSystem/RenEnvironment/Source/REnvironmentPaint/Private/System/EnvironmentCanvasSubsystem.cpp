// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "System/EnvironmentCanvasSubsystem.h"

// Engine Headers
#include "Engine/AssetManager.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

// Project Headers
#include "Component/EnvironmentBrushComponent.h"
#include "Core/AssetManagerUtil.h"
#include "Data/EnvironmentPaintWorldConfig.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "Util/MiscUtil.h"
#include "WorldFragmentSettings.h"


void UEnvironmentCanvasSubsystem::RegisterBrush(UEnvironmentBrushComponent* Component)
{
	if (BrushLimit > 0 && BrushCollection.Num() >= BrushLimit)
	{
		LOG_WARNING(LogEnvironmentPaint, TEXT("Brushes limit reached"));
		return;
	}

	if (Component && !BrushCollection.Contains(Component))
	{
		BrushToAdd.Add(TWeakInterfacePtr<UEnvironmentBrushComponent>(Component));
	}
}

void UEnvironmentCanvasSubsystem::UnregisterBrush(UEnvironmentBrushComponent* Component)
{
	if (Component && !BrushCollection.Contains(Component))
	{
		BrushToRemove.Add(TWeakInterfacePtr<UEnvironmentBrushComponent>(Component));
	}
}


#if WITH_EDITOR
void UEnvironmentCanvasSubsystem::DrawDebug()
{
	if (bDrawDebug)
	{
		FVector StartLocation = NiagaraLocation;
		FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 100.0f);

		DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 10.0f, FColor::Magenta, false, -1.0f, 1, 10.0f);
		DrawDebugBox(GetWorld(), NiagaraLocation, FVector(CanvasSize / 2), FColor::Magenta, false, -1.0f, 0, 10.0f);
	}
}
#endif


void UEnvironmentCanvasSubsystem::ResolvePendingBrush()
{
	if (BrushToAdd.Num() > 0)
	{
		BrushCollection.Append(BrushToAdd);
		BrushToAdd.Reset();
	}

	if (BrushToRemove.Num() > 0)
	{
		BrushCollection.RemoveAll(
			[this](const TWeakInterfacePtr<UEnvironmentBrushComponent>& Item)
			{
				return BrushToRemove.Contains(Item);
			}
		);
		BrushToRemove.Reset();
	}
}

void UEnvironmentCanvasSubsystem::MoveRenderTarget()
{
	PixelOffset = GetPixelOffset();

	NiagaraComponent->SetVariableVec2(CanvasParameter.NS_PixelOffset, (PixelOffset / CanvasSize));
	NiagaraLocation = NiagaraLocation + FVector(PixelOffset.X, PixelOffset.Y, 0.0f);

	MPCInstance->SetVectorParameterValue(CanvasParameter.MPC_CanvasLocation, NiagaraLocation);
}

void UEnvironmentCanvasSubsystem::DrawRenderTarget()
{
	int PointCount = 0;

	TArray<FVector4> BrushPoints;
	BrushPoints.Reserve(PointLimit);

	for (TWeakInterfacePtr<UEnvironmentBrushComponent>& Item : BrushCollection)
	{
		if (PointCount > PointLimit)
		{
			break;
		}

		UEnvironmentBrushComponent* Brush = Item.Get();
		if (!Brush)
		{
			continue;
		}

		FVector Location = FVector::ZeroVector;
		FVector2D Size = FVector2D::ZeroVector;
		float Density = 0.0f;

		bool bCanDraw = Brush->GetBrushDetails(Location, Size, Density);
		if (!bCanDraw)
		{
			continue;
		}

		FVector DrawLocation = Location - NiagaraLocation;

		float PackedDensity = FMiscUtil::PackFloats(Density, 1.0f);
		float PackedSize = FMiscUtil::PackFloats(Size.Length(), 1.0f);
		BrushPoints.Add(FVector4(DrawLocation.X, DrawLocation.Y, PackedSize, PackedDensity));
		PointCount++;
	}

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector4(NiagaraComponent, CanvasParameter.NS_DrawPoints, BrushPoints);
}


bool UEnvironmentCanvasSubsystem::InitializeNiagara(UNiagaraSystem* System, ENiagaraTickBehavior TickBehaviour)
{
	if (!IsValid(System) || IsValid(NiagaraComponent))
	{
		return false;
	}

	FFXSystemSpawnParameters SpawnParameters;
	SpawnParameters.bAutoActivate = false;
	SpawnParameters.WorldContextObject = GetWorld();
	SpawnParameters.SystemTemplate = System;
	SpawnParameters.Location = NiagaraLocation;
	
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocationWithParams(SpawnParameters);
	NiagaraComponent->SetTickBehavior(TickBehaviour);

	return IsValid(NiagaraComponent);
}

void UEnvironmentCanvasSubsystem::DeinitializeNiagara()
{
	if (!IsValid(NiagaraComponent))
	{
		return;
	}
	NiagaraComponent->Deactivate();
}


bool UEnvironmentCanvasSubsystem::InitializeMPC(const UMaterialParameterCollection* Collection)
{
	MPCInstance = GetWorld()->GetParameterCollectionInstance(Collection);
	if (!IsValid(MPCInstance))
	{
		PRINT_ERROR(LogEnvironmentPaint, 1.0f, TEXT("MPCInstance is invalid"));
		return false;
	}

	MPCInstance->SetScalarParameterValue(CanvasParameter.MPC_CanvasSize, CanvasSize);
	MPCInstance->SetVectorParameterValue(CanvasParameter.MPC_CanvasLocation, NiagaraLocation);
	return true;
}

bool UEnvironmentCanvasSubsystem::InitializePixelRatio()
{
	if (CanvasSize <= 0.0f || RenderTargetSize <= 0.0f)
	{
		LOG_ERROR(LogEnvironmentPaint, TEXT("CanvasSize or ImageSize is invalid"));
		return false;
	}

	PixelRatio = (1.0 / RenderTargetSize) * CanvasSize;
	NiagaraComponent->SetVariableFloat(CanvasParameter.NS_PixelRatio, PixelRatio);
	return true;
}

bool UEnvironmentCanvasSubsystem::ClearRenderTarget(UTextureRenderTarget2D* MainRT, UTextureRenderTarget2D* PersistentRT)
{
	if (!IsValid(MainRT) || !IsValid(PersistentRT))
	{
		return false;
	}

	FLinearColor ClearColor = FLinearColor::Black;
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), MainRT, ClearColor);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), PersistentRT, ClearColor);
	return true;
}


bool UEnvironmentCanvasSubsystem::InitializeController()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		PRINT_ERROR(LogEnvironmentPaint, 1.0f, TEXT("PlayerController is invalid"));
		return false;
	}

	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UEnvironmentCanvasSubsystem::HandleOnPawnChanged);

	CameraManager = PlayerController->PlayerCameraManager;
	Controller = PlayerController;
	Pawn = PlayerController->GetPawn();

	return true;
}

void UEnvironmentCanvasSubsystem::DeinitializeController()
{
	APlayerController* PlayerController = Controller.Get();
	if (IsValid(PlayerController))
	{
		PlayerController->OnPossessedPawnChanged.RemoveAll(this);
	}

	CameraManager = nullptr;
	Controller = nullptr;
	Pawn = nullptr;
}


void UEnvironmentCanvasSubsystem::HandleOnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Pawn = NewPawn;
}

void UEnvironmentCanvasSubsystem::HandleOnEnvironmentCanvasLoaded()
{
	const UEnvironmentPaintWorldConfig* Config = GetPainWorldConfig(GetWorld());
	if (!IsValid(Config))
	{
		return;
	}

	CanvasParameter = Config->CanvasParameter;
	RenderTargetSize = Config->RenderTargetSize;
	CanvasSize = Config->CanvasSize;
	BrushLimit = Config->BrushLimit;
	PointLimit = Config->PointLimit;

	if (!InitializeNiagara(Config->NiagaraSystem.Get(), Config->NiagaraTick))
	{
		LOG_ERROR(LogEnvironmentPaint, TEXT("Failed to created niagara system"));
		return;
	}

	bool bMPC = InitializeMPC(Config->MPC.Get());
	bool bPixelRatio = InitializePixelRatio();
	bool bController = InitializeController();
	bool bRenderTarget = ClearRenderTarget(Config->MainRT.Get(), Config->PersistentRT.Get());
	if (!bMPC || !bPixelRatio || !bController || !bRenderTarget)
	{
		LOG_ERROR(LogEnvironmentPaint, TEXT("Initialization condition failed during canvas creation"));
		return;
	}

	bInitializedSuccessfully = true;
	HandleOnEnvironmentCVarChanged(EnvironmentPaintCVar);
}

void UEnvironmentCanvasSubsystem::HandleOnEnvironmentCVarChanged(IConsoleVariable* Variable)
{
	if (!bInitializedSuccessfully)
	{
		return;
	}

#if WITH_EDITOR
	if (EnvironmentPaintDebugCVar && Variable == EnvironmentPaintDebugCVar)
	{
		bDrawDebug = EnvironmentPaintDebugCVar->GetBool();
	}
#endif

	if (EnvironmentPaintCVar && Variable == EnvironmentPaintCVar)
	{
		bIsDrawing = (EnvironmentPaintCVar->GetInt() > 0);
		if (bIsDrawing)
		{
			if (!NiagaraComponent->IsActive())
			{
				NiagaraComponent->Activate();
			}
			else if (NiagaraComponent->IsPaused())
			{
				NiagaraComponent->SetPaused(false);
			}
		}
		else
		{
			if (NiagaraComponent->IsActive() && !NiagaraComponent->IsPaused())
			{
				NiagaraComponent->SetPaused(true);
			}
		}
	}
}


FVector2D UEnvironmentCanvasSubsystem::GetPixelOffset() const
{
	APlayerCameraManager* Camera = CameraManager.Get();
	if (!IsValid(Camera))
	{
		return FVector2D::ZeroVector;
	}

	FVector PlayerLocation = Camera->GetCameraLocation();

	float X = FMath::Floor(PlayerLocation.X / PixelRatio);
	float Y = FMath::Floor(PlayerLocation.Y / PixelRatio);

	FVector2D Snap2D = (FVector2D(X, Y) + 0.5f) * PixelRatio;
	FVector2D Location2D = FVector2D(NiagaraLocation.X, NiagaraLocation.Y);

	return (Snap2D - Location2D);
}

const UEnvironmentPaintWorldConfig* UEnvironmentCanvasSubsystem::GetPainWorldConfig(UWorld* World) const
{
	AWorldFragmentSettings* WorldSettings = Cast<AWorldFragmentSettings>(World->GetWorldSettings());
	if (!IsValid(WorldSettings))
	{
		return nullptr;
	}
	return WorldSettings->FindConfigByClass<UEnvironmentPaintWorldConfig>();
}


TStatId UEnvironmentCanvasSubsystem::GetStatId() const
{
	return GetStatID();
}

void UEnvironmentCanvasSubsystem::Tick(float DeltaTime)
{
	ResolvePendingBrush();
	MoveRenderTarget();
	DrawRenderTarget();

#if WITH_EDITOR
	DrawDebug();
#endif
}

bool UEnvironmentCanvasSubsystem::IsTickable() const
{
	return bIsDrawing && bInitializedSuccessfully;
}


bool UEnvironmentCanvasSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

bool UEnvironmentCanvasSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	const UEnvironmentPaintWorldConfig* Config = GetPainWorldConfig(Cast<UWorld>(Outer));
	if (!IsValid(Config))
	{
		return false;
	}

	return Config->bEnabled;
}

void UEnvironmentCanvasSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LOG_WARNING(LogEnvironmentPaint, TEXT("EnvironmentCanvasSubsystem Initialized"));

	EnvironmentPaintCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Paint"));
	if (EnvironmentPaintCVar)
	{
		EnvironmentPaintCVar->OnChangedDelegate().AddUObject(this, &UEnvironmentCanvasSubsystem::HandleOnEnvironmentCVarChanged);
	}

#if WITH_EDITOR
	EnvironmentPaintDebugCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("ren.Environment.Paint.Debug"));
	if (EnvironmentPaintDebugCVar)
	{
		EnvironmentPaintDebugCVar->OnChangedDelegate().AddUObject(this, &UEnvironmentCanvasSubsystem::HandleOnEnvironmentCVarChanged);
	}
#endif
}

void UEnvironmentCanvasSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	LOG_WARNING(LogEnvironmentPaint, TEXT("EnvironmentCanvasSubsystem OnWorldBeginPlay"));

	const UEnvironmentPaintWorldConfig* Config = GetPainWorldConfig(&InWorld);
	if (!IsValid(Config) || !Config->bEnabled)
	{
		LOG_WARNING(LogEnvironmentPaint, TEXT("Environment paint is invalid or disabled"));
		return;
	}

	TArray<FSoftObjectPath> SoftObjects;
	SoftObjects.Add(Config->NiagaraSystem.ToSoftObjectPath());
	SoftObjects.Add(Config->MainRT.ToSoftObjectPath());
	SoftObjects.Add(Config->PersistentRT.ToSoftObjectPath());
	SoftObjects.Add(Config->MPC.ToSoftObjectPath());

	FStreamableManager& Manager = UAssetManager::GetStreamableManager();
	StreamHandle = Manager.RequestAsyncLoad(SoftObjects, FStreamableDelegate::CreateUObject(this, &UEnvironmentCanvasSubsystem::HandleOnEnvironmentCanvasLoaded));
}

void UEnvironmentCanvasSubsystem::OnWorldEndPlay(UWorld& InWorld)
{
	if (EnvironmentPaintCVar)
	{
		EnvironmentPaintCVar->OnChangedDelegate().RemoveAll(this);
	}
	EnvironmentPaintCVar = nullptr;

	bIsDrawing = false;
	bInitializedSuccessfully = false;
	FAssetManagerUtil::CancelHandle(StreamHandle);

	DeinitializeNiagara();
	DeinitializeController();

	LOG_WARNING(LogEnvironmentPaint, TEXT("EnvironmentCanvasSubsystem OnWorldEndPlay"));
	Super::OnWorldEndPlay(InWorld);
}

void UEnvironmentCanvasSubsystem::Deinitialize()
{
	LOG_WARNING(LogEnvironmentPaint, TEXT("EnvironmentCanvasSubsystem Deinitialized"));
	Super::Deinitialize();
}


UEnvironmentCanvasSubsystem* UEnvironmentCanvasSubsystem::Get(UWorld* World)
{
	if (!IsValid(World))
	{
		return nullptr;
	}
	return World->GetSubsystem<UEnvironmentCanvasSubsystem>();
}


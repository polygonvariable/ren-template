// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EnvironmentRegionActor.h"

// Engine Headers
#include "Components/PrimitiveComponent.h"
#include "Engine/AssetManager.h"

// Project Headers
#include "Core/AssetManagerUtil.h"
#include "Core/EnvironmentSettings.h"
#include "Data/EnvironmentProfileAsset.h"
#include "Log/LogCategory.h"
#include "Log/LogMacro.h"
#include "System/EnvironmentSubsystem.h"


void AEnvironmentRegionActor::LoadProfile()
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(AssetManager))
	{
		return;
	}

	FAssetManagerUtil::CancelHandle(AssetHandle);

	TArray<FPrimaryAssetId> AssetIds;
	ProfileAssets.GetKeys(AssetIds);

	const UEnvironmentSettings* Settings = UEnvironmentSettings::Get();
	const TArray<FName>& Bundle = Settings->EnvironmentBundles;

	AssetHandle = AssetManager->LoadPrimaryAssets(AssetIds, Bundle, FStreamableDelegate::CreateUObject(this, &AEnvironmentRegionActor::HandleOnProfileLoaded));
}

void AEnvironmentRegionActor::AddProfile()
{
	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(EnvironmentSubsystem) || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Environment subsystem or asset manager is invalid"));
		return;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : ProfileAssets)
	{
		EnvironmentSubsystem->AddProfile(AssetManager->GetPrimaryAssetObject<UEnvironmentProfileAsset>(Kv.Key), Kv.Value);
	}
}

void AEnvironmentRegionActor::RemoveProfile()
{
	FAssetManagerUtil::CancelHandle(AssetHandle);

	UAssetManager* AssetManager = UAssetManager::GetIfInitialized();
	if (!IsValid(EnvironmentSubsystem) || !IsValid(AssetManager))
	{
		PRINT_ERROR(LogEnvironment, 1.0f, TEXT("Environment subsystem or asset manager is invalid"));
		return;
	}

	for (const TPair<FPrimaryAssetId, int>& Kv : ProfileAssets)
	{
		EnvironmentSubsystem->RemoveProfile(AssetManager->GetPrimaryAssetObject<UEnvironmentProfileAsset>(Kv.Key), Kv.Value);
	}
}


void AEnvironmentRegionActor::HandleOnProfileLoaded()
{
	FAssetManagerUtil::ReleaseHandle(AssetHandle);
	AddProfile();
}

void AEnvironmentRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor) && !bPlayerInRegion)
	{
		bPlayerInRegion = true;
		LoadProfile();
	}
}

void AEnvironmentRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor) && bPlayerInRegion)
	{
		bPlayerInRegion = false;
		RemoveProfile();
	}
}


void AEnvironmentRegionActor::BeginPlay()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		EnvironmentSubsystem = World->GetSubsystem<UEnvironmentSubsystem>();
	}

	Super::BeginPlay();
}

void AEnvironmentRegionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveProfile();
	EnvironmentSubsystem = nullptr;

	Super::EndPlay(EndPlayReason);
}


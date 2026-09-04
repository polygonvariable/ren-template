// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/EnvironmentRegionActor.h"

// Engine Headers
#include "Components/PrimitiveComponent.h"

// Project Headers
#include "Log/LogMacro.h"
#include "System/EnvironmentSubsystem.h"


void AEnvironmentRegionActor::AddProfile()
{
	if (!IsValid(EnvironmentSubsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
		return;
	}

	for (const TPair<TObjectPtr<UEnvironmentProfileAsset>, int>& Kv : ProfileAssets)
	{
		EnvironmentSubsystem->AddProfile(Kv.Key, Kv.Value);
	}
}

void AEnvironmentRegionActor::RemoveProfile()
{
	if (!IsValid(EnvironmentSubsystem))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
		return;
	}

	for (const TPair<TObjectPtr<UEnvironmentProfileAsset>, int>& Kv : ProfileAssets)
	{
		EnvironmentSubsystem->RemoveProfile(Kv.Key, Kv.Value);
	}
}

void AEnvironmentRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (DoesCollidedWithPlayer(OtherActor))
	{
		AddProfile();
	}
}

void AEnvironmentRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (DoesCollidedWithPlayer(OtherActor))
	{
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


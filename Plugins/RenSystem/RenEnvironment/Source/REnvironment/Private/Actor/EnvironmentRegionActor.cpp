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
	UEnvironmentSubsystem* SubsystemPtr = EnvironmentSubsystem.Get();
	if (!IsValid(SubsystemPtr))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
		return;
	}

	for (auto& Kvp : ProfileAssets)
	{
		//SubsystemPtr->AddStackedProfile(Kvp.Key, Kvp.Value);
	}
}

void AEnvironmentRegionActor::RemoveProfile()
{
	UEnvironmentSubsystem* SubsystemPtr = EnvironmentSubsystem.Get();
	if (!IsValid(SubsystemPtr))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
		return;
	}

	for (auto& Kvp : ProfileAssets)
	{
		//SubsystemPtr->RemoveStackedProfile(Kvp.Key, Kvp.Value);
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
	EnvironmentSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}


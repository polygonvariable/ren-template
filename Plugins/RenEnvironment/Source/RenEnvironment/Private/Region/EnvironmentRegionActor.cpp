// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Region/EnvironmentRegionActor.h"

// Engine Headers
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Project Headers
#include "RenCore/Public/Macro/LogMacro.h"

#include "RenEnvironment/Public/Asset/EnvironmentProfileAsset.h"
#include "RenEnvironment/Public/Subsystem/EnvironmentSubsystem.h"



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
		SubsystemPtr->AddStackedProfile(Kvp.Key, Kvp.Value);
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
		SubsystemPtr->RemoveStackedProfile(Kvp.Key, Kvp.Value);
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


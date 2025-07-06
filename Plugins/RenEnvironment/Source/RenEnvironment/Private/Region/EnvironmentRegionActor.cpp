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


void AEnvironmentRegionActor::RegisterCollisionComponent(UPrimitiveComponent* CollisionComponent)
{
	if (!IsValid(CollisionComponent))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid collision component"));
		return;
	}

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnvironmentRegionActor::HandlePlayerEntered);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AEnvironmentRegionActor::HandlePlayerExited);
}

void AEnvironmentRegionActor::UnregisterCollisionComponent(UPrimitiveComponent* CollisionComponent)
{
	if (!IsValid(CollisionComponent))
	{
		LOG_ERROR(LogTemp, TEXT("Invalid collision component"));
		return;
	}

	CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AEnvironmentRegionActor::HandlePlayerEntered);
	CollisionComponent->OnComponentEndOverlap.RemoveDynamic(this, &AEnvironmentRegionActor::HandlePlayerExited);
}

void AEnvironmentRegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ACharacter* CollidedCharacter = Cast<ACharacter>(OtherActor);

	if (PlayerCharacter != CollidedCharacter)
	{
		LOG_WARNING(LogTemp, TEXT("Actor entered region, but not player character"));
		return;
	}

	UEnvironmentSubsystem* SubsystemPtr = EnvironmentSubsystem.Get();
	if (IsValid(SubsystemPtr))
	{
		for (auto& Kvp : ProfileAssets)
		{
			SubsystemPtr->AddStackedProfile(Kvp.Value, Kvp.Key);
		}
	}
	else
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
	}
}

void AEnvironmentRegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	UEnvironmentSubsystem* SubsystemPtr = EnvironmentSubsystem.Get();
	if (IsValid(SubsystemPtr))
	{
		for (auto& Kvp : ProfileAssets)
		{
			SubsystemPtr->RemoveStackedProfile(Kvp.Value, Kvp.Key);
		}
	}
	else
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid environment subsystem"));
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
	HandlePlayerExited(nullptr, nullptr, nullptr, 0);
	EnvironmentSubsystem.Reset();

	Super::EndPlay(EndPlayReason);
}


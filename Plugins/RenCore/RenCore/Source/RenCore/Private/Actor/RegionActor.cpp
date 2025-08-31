// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "Actor/RegionActor.h"

// Engine Headers
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Project Headers
#include "RenCoreLibrary/Public/LogMacro.h"



ARegionActor::ARegionActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

UPrimitiveComponent* ARegionActor::GetCollisionComponent_Implementation() const
{
	return nullptr;
}

void ARegionActor::RegisterCollisionComponent()
{
	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (!IsValid(CollisionComponent))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid collision component"));
		return;
	}

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARegionActor::HandlePlayerEntered);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ARegionActor::HandlePlayerExited);
}

void ARegionActor::UnregisterCollisionComponent()
{
	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (!IsValid(CollisionComponent))
	{
		PRINT_ERROR(LogTemp, 1.0f, TEXT("Invalid collision component"));
		return;
	}

	CollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
	CollisionComponent->OnComponentEndOverlap.RemoveAll(this);
}

void ARegionActor::EnableCollision(bool bEnable)
{
	UPrimitiveComponent* CollisionComponent = GetCollisionComponent();
	if (IsValid(CollisionComponent))
	{
		ECollisionEnabled::Type Collision = bEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;
		CollisionComponent->SetCollisionEnabled(Collision);
	}
}

bool ARegionActor::DoesCollidedWithPlayer(AActor* OtherActor) const
{
	if (!IsValid(OtherActor))
	{
		return false;
	}
	
	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	return PlayerCharacter == OtherCharacter;
}


void ARegionActor::HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ARegionActor::HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{

}


void ARegionActor::BeginPlay()
{
	RegisterCollisionComponent();

	Super::BeginPlay();
}

void ARegionActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterCollisionComponent();

	Super::EndPlay(EndPlayReason);
}


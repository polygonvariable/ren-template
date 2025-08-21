// Fill out your copyright notice in the Description page of Project Settings.

// Parent Header
#include "PickupActor.h"

// Engine Headers
#include "Components/SphereComponent.h"

// Project Headers
#include "RenInventory/Public/InventorySubsystem.h"
#include "RenTimestamp/Public/TimestampCooldownComponent.h"



APickupActor::APickupActor()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	if (IsValid(SphereCollision))
	{
		SphereCollision->SetupAttachment(RootComponent);
		SphereCollision->SetSphereRadius(150.0f);

#if WITH_EDITOR
		SphereCollision->SetLineThickness(2.0f);
		SphereCollision->SetHiddenInGame(false);
		SphereCollision->ShapeColor = FColor::Green;
#endif
	}
}

UPrimitiveComponent* APickupActor::GetCollisionComponent_Implementation() const
{
	return SphereCollision;
}



ASpawnablePickupActor::ASpawnablePickupActor()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	TimestampComponent = CreateDefaultSubobject<UTimestampCooldownComponent>(TEXT("TimestampComponent"));
	if (IsValid(TimestampComponent))
	{
		TimestampComponent->bAutoActivate = true;
	}
}

void ASpawnablePickupActor::GenerateTimestampId()
{
	if (IsValid(TimestampComponent))
	{
		FGuid Guid = FGuid::NewGuid();
		TimestampComponent->TimestampId = FName(*Guid.ToString());
	}
}

void ASpawnablePickupActor::BeginPlay()
{
	if (IsValid(TimestampComponent))
	{
		ETimestampCooldownStatus Status = TimestampComponent->GetCooldownStatus();
		switch (Status)
		{
		case ETimestampCooldownStatus::Completed:
		case ETimestampCooldownStatus::NotFound:
			SetActorEnableCollision(true);
			SetActorHiddenInGame(false);
			break;

		case ETimestampCooldownStatus::Once:
		case ETimestampCooldownStatus::Pending:
		default:
			SetActorEnableCollision(false);
			SetActorHiddenInGame(true);
			break;
		}
	}

	Super::BeginPlay();
}



void AInventoryPickupActor::Interacted()
{
	if (TimestampComponent) TimestampComponent->AddTimestamp(false);

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	/*if (bHideAfterPickup)
	{
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);
	}
	else
	{
		if (SphereCollision)
		{
			SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}*/
}

UInventorySubsystem* AInventoryPickupActor::GetInventorySubsystem() const
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance) return nullptr;

	return GameInstance->GetSubsystem<UInventorySubsystem>();
}



void AInventoryItemActor::Interacted()
{
	UInventorySubsystem* InventorySubsystem = GetInventorySubsystem();
	if (!InventorySubsystem) return;
	if (!InventorySubsystem->AddItem(ContainerId, InventoryAsset, ItemQuantity)) return;

	AInventoryPickupActor::Interacted();
}

void AInventoryChestActor::Interacted()
{
	UInventorySubsystem* InventorySubsystem = GetInventorySubsystem();
	if (!InventorySubsystem) return;
	if (!InventorySubsystem->AddItems(ContainerId, InventoryAssets)) return;

	AInventoryPickupActor::Interacted();
}


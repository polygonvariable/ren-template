// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers

// Generated Headers
#include "RegionActor.generated.h"

// Forward Declarations
class UPrimitiveComponent;



/**
 * 
 * 
 */
UCLASS(MinimalAPI)
class ARegionActor : public AActor
{

	GENERATED_BODY()

public:

	RENCORE_API ARegionActor();
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Region Actor")
	bool bPlayerInRegion = false;


	UFUNCTION(BlueprintNativeEvent)
	UPrimitiveComponent* GetCollisionComponent() const;
	RENCORE_API virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const;

	RENCORE_API virtual void RegisterCollisionComponent();
	RENCORE_API virtual void UnregisterCollisionComponent();
	RENCORE_API virtual bool DoesCollidedWithPlayer(AActor* OtherActor) const;

	UFUNCTION()
	RENCORE_API virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	RENCORE_API virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

protected:

	RENCORE_API virtual void BeginPlay() override;
	RENCORE_API virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};


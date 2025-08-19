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
UCLASS(Abstract)
class RENCORE_API ARegionActor : public AActor
{

	GENERATED_BODY()

public:

	ARegionActor();
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Region Actor")
	bool bPlayerInRegion = false;


	UFUNCTION(BlueprintNativeEvent)
	UPrimitiveComponent* GetCollisionComponent() const;
	virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const;

	virtual void RegisterCollisionComponent();
	virtual void UnregisterCollisionComponent();
	virtual bool DoesCollidedWithPlayer(AActor* OtherActor) const;

	UFUNCTION()
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};


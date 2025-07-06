// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers

// Generated Headers
#include "EnvironmentRegionActor.generated.h"

// Forward Declarations
class UPrimitiveComponent;
class UEnvironmentSubsystem;
class UEnvironmentProfileAsset;



/**
 * TODO
 * Make a separate collision profile for character interaction,
 * so other actors can be ignored
 */
UCLASS()
class AEnvironmentRegionActor : public AActor
{

	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, TObjectPtr<UEnvironmentProfileAsset>> ProfileAssets;

	UPROPERTY()
	TWeakObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;


	UFUNCTION(BlueprintCallable)
	void RegisterCollisionComponent(UPrimitiveComponent* CollisionComponent);

	UFUNCTION(BlueprintCallable)
	void UnregisterCollisionComponent(UPrimitiveComponent* CollisionComponent);


	UFUNCTION()
	void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};


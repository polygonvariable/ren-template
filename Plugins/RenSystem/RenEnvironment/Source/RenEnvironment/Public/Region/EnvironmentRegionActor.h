// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Project Headers
#include "RCoreCommon/Public/Actor/RegionActor.h"

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
class AEnvironmentRegionActor : public ARegionActor
{

	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TObjectPtr<UEnvironmentProfileAsset>, int> ProfileAssets;

	UPROPERTY()
	TWeakObjectPtr<UEnvironmentSubsystem> EnvironmentSubsystem;


	void AddProfile();
	void RemoveProfile();


	void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex) override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};


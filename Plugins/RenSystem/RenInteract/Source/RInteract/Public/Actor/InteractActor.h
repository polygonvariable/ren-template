// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/InteractItem.h"
#include "Interface/InteractInterface.h"

// Generated Headers
#include "InteractActor.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UPrimitiveComponent;
class UInteractSubsystem;


/**
 *
 */
UCLASS(Abstract)
class AInteractActor : public AActor, public IInteractInterface
{

	GENERATED_BODY()
	
public:

	AInteractActor();

	// ~ AActor
	void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of AActor

	// ~ IInteractInterface
	virtual void OnInteracted() override;
	// ~ End of IInteractInterface

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid InteractId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInteractItem InteractItem;

	UPROPERTY()
	TObjectPtr<UInteractSubsystem> InteractSubsystem = nullptr;


	UFUNCTION(BlueprintCallable)
	void UpdateInteract();

	UFUNCTION(BlueprintNativeEvent)
	UPrimitiveComponent* GetCollisionComponent() const;
	virtual UPrimitiveComponent* GetCollisionComponent_Implementation() const;

	UFUNCTION(BlueprintNativeEvent)
	bool CollisionCondition(AActor* Actor) const;
	virtual bool CollisionCondition_Implementation(AActor* Actor) const;

	UFUNCTION()
	virtual void HandlePlayerEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void HandlePlayerExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex);

	// ~ AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~ End of AActor

private:

	bool _bIsInteracting = false;

};


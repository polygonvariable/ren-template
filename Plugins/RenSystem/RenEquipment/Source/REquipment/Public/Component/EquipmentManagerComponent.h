// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Definition/EquipmentSpawnData.h"

// Generated Headers
#include "EquipmentManagerComponent.generated.h"

// Forward Declarations
class URAssetManager;
class UEquipmentStorage;
class UEquipmentSubsystem;
class UActorFreeListSubsystem;
class AEquipmentActor;


/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UEquipmentManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	TArray<FEquipmentData> SpawnData;


	UFUNCTION(BlueprintCallable)
	void SpawnEquipment();

	UFUNCTION(BlueprintCallable)
	void RemoveEquipment();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

protected:

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(EditAnywhere)
	EAssetQuerySource SourceType = EAssetQuerySource::Instance;

	UPROPERTY()
	URAssetManager* AssetManager = nullptr;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<AEquipmentActor>> SpawnedEquipment;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UActorFreeListSubsystem> ActorFreeList = nullptr;


	void SyncOwnerEquipment(const FGuid& InOwnerId);

	void UpdateSpawnData();
	void SpawnEquipment_Internal();

private:

	FGuid _EquipmentSpawnId;

};


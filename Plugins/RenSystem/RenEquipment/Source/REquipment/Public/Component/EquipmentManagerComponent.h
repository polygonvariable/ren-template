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
class FObjectPreSaveContext;
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

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<FEquipmentSpawnData> SpawnData;


	UFUNCTION(BlueprintCallable)
	void SpawnEquipment();

	UFUNCTION(BlueprintCallable)
	void RemoveEquipment();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY(VisibleAnywhere)
	FGuid OwnerId;

	UPROPERTY(EditAnywhere)
	EAssetQuerySource EquipmentSource = EAssetQuerySource::Instance;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, Meta = (DisplayName = "Spawn Data (Editor)"))
	TArray<FEquipmentSpawnData> SpawnDataEd;

#endif
	
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

	void GetInstancedSpawnData();
	void SpawnEquipment_Internal();

private:

	FGuid _EquipmentSpawnId;

};


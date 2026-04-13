// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Definition/EquipmentData.h"

// Generated Headers
#include "EquipmentManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class UEquipmentStorage;
class UEquipmentSubsystem;
class UActorFreeListSubsystem;
class AEquipmentActor;
class UAbilitySystemComponent;
class UEquipmentController;


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
	TArray<FEquipmentData> EquipmentSpawnData;


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

	UPROPERTY(EditAnywhere)
	EAssetQuerySource SourceType = EAssetQuerySource::Asset;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	FGuid EquipmentOwnerId;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TMap<FEquipmentData, TObjectPtr<UEquipmentController>> EquippedControllers;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<FPrimaryAssetId> EquippedAssetIds;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UActorFreeListSubsystem> ActorFreeList = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerASC = nullptr;


	void SyncEquipment(const FGuid& InOwnerId);
	void SpawnEquipmentActors();

	void RefreshEquipmentData();
	void CleanupEquipmentActors();

	void RegisterEquipment(const FEquipmentData& Data, UEquipmentController* Controller);
	void UnregisterEquipment(const FEquipmentData& Data);

private:

	FGuid _SpawnId;

};


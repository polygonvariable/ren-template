// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/AssetQuerySource.h"
#include "Definition/EquipmentData.h"
#include "Definition/PoolCollection.h"

// Generated Headers
#include "EquipmentManagerComponent.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class UEquipmentStorage;
class UEquipmentSubsystem;
class UActorFreelistSubsystem;
class UEquipmentController;


/**
 *
 *
 */
UCLASS(MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UEquipmentManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditAnywhere)
	TArray<FEquipmentData> EquipmentSpawnData;


	DECLARE_MULTICAST_DELEGATE(FOnEquipmentChanged);
	FOnEquipmentChanged OnEquipmentChangeBegin;
	FOnEquipmentChanged OnEquipmentChangeEnd;


	UFUNCTION(BlueprintCallable)
	void SpawnEquipment();

	UFUNCTION(BlueprintCallable)
	void RemoveEquipment();


	REN_API UEquipmentController* GetEquipmentControllerByTag(const FGameplayTag& Tag) const;


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

	UPROPERTY()
	FGuid OwnerInstanceId;

	UPROPERTY()
	TMap<FEquipmentData, TObjectPtr<UEquipmentController>> EquippedControllers;

	UPROPERTY()
	TArray<FPrimaryAssetId> EquippedAssetIds;

	UPROPERTY()
	TObjectPtr<UEquipmentStorage> EquipmentStorage = nullptr;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UActorFreelistSubsystem> ActorFreelist = nullptr;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;



	void SyncEquipment(const FGuid& InOwnerId);
	void SpawnEquipmentActors();

	void RefreshEquipmentData();
	void CleanupEquipmentData();

	void RegisterEquipment(const FEquipmentData& Data, UEquipmentController* Controller);
	void UnregisterEquipment(const FEquipmentData& Data);

	bool GetIsSpawning() const;
	void SetIsSpawning(bool bIsSpawning);

private:

	FGuid _SpawnId;
	bool _bIsSpawning = false;

	UPROPERTY()
	TMap<UClass*, FPoolCollection> _ControllerPool;

};

// Module Macros
#undef REN_API


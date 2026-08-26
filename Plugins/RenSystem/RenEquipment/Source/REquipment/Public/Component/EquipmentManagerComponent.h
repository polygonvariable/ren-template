// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSpawnData.h"
#include "Definition/PoolCollection.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "EquipmentManagerComponent.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class FObjectPreSaveContext;
class UAssetManager;
class UEquipmentStorageManager;
class UEquipmentSubsystem;
class UActorFreelistSubsystem;
class UEquipmentController;
struct FStreamableHandle;
struct FEquipmentSlotDefinition;
class UAnimInstance;
class UEquipmentStateController;


/**
 *
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UEquipmentManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceType==EDataSource::Static", EditConditionHides))
	TArray<FEquipmentInitializationData> EquipmentSpawnData;

	DECLARE_MULTICAST_DELEGATE(FOnEquipmentManagerEvent);
	FOnEquipmentManagerEvent OnEquipmentReset;
	FOnEquipmentManagerEvent OnEquipmentUpdate;


	UFUNCTION(BlueprintCallable)
	virtual void InitializeManager();

	UFUNCTION(BlueprintCallable)
	virtual void DeinitializeManager();

	UFUNCTION(BlueprintCallable)
	REN_API void ActivateEquipmentById(FGameplayTag CategoryTag, int SlotId);
	REN_API UEquipmentController* GetEquipmentControllerByTag(const FEquipmentSlotDefinition& SlotDefinition) const;


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
	TObjectPtr<UEquipmentStateController> PendingController = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEquipmentStateController> CurrentController = nullptr;

	UPROPERTY(EditAnywhere)
	EDataSource SourceType = EDataSource::Static;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UEquipmentController>> EquipmentControllers;

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem = nullptr;

	UPROPERTY()
	TObjectPtr<UActorFreelistSubsystem> ActorFreelist = nullptr;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager = nullptr;

	FGuid OwnerInstanceId;

	int CurrentIndex = 0;

	void UpdateEquipment(const FGuid& InOwnerId);
	void CreateEquipment();
	void RemoveEquipment();
	void SpawnEquipmentActors();

	void RefreshEquipmentData(TArray<FPrimaryAssetId>& OutAssetIds);
	void CleanupEquipmentData();
	void RegisterEquipment(UEquipmentController* Controller);
	void UnregisterEquipment(UEquipmentController* Controller);

	void HandleControllerQueue();
	void ActivatePendingController();
	void RemovePendingController();
	void RemoveCurrentController();

	void BindController(UEquipmentStateController* Controller);
	void UnbindController(UEquipmentStateController* Controller);

	// ~ Bindings
	void HandleOnEquipmentActivated(UEquipmentStateController* Controller);
	void HandleOnEquipmentDeactivated(UEquipmentStateController* Controller);
	// ~ End od Binding

	bool IsInitialized() const
	{
		return _bInitialized;
	}

	void SetInitialized(bool bValue)
	{
		_bInitialized = bValue;
	}

private:

	TSharedPtr<FStreamableHandle> _SpawnHandle = nullptr;

	UPROPERTY(VisibleAnywhere)
	TMap<UClass*, FPoolCollection> _ControllerPool;
	
	bool _bInitialized = false;

};

// Module Macros
#undef REN_API


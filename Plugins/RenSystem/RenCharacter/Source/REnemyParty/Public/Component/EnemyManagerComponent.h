// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/EnemyData.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "EnemyManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class UPartySubsystem;
class UPartyStorageManager;
class AEnemyCharacter;


/**
 *
 *
 */
UCLASS(MinimalAPI, Meta = (BlueprintSpawnableComponent))
class UEnemyManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEnemyManagerComponent(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(EditAnywhere, Category = "Collection")
	TArray<FEnemySpawnData> SpawnData;


	UFUNCTION(BlueprintCallable)
	void TrySpawnEnemies();

	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY(EditAnywhere, Category = "Source")
	EDataSource SourceType = EDataSource::Static;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Collection")
	TMap<FGuid, TObjectPtr<AEnemyCharacter>> ActiveEnemies;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager = nullptr;


	void GetSpawningAssetIds(TArray<FPrimaryAssetId>& OutAssetIds) const;

	void SpawnEnemies();
	void SpawnEnemy(const FEnemySpawnData& Data);

	void RegisterEnemy(FGuid EnemyId, AEnemyCharacter* Character);
	void UnregisterEnemy(AEnemyCharacter* Character);

	void CleanupEnemies();
	
	void OnEnemyDataUpdated();

	int GetRemainingEnemiesCount() const;

private:

	FGuid _SpawnId;
	FVector _SpawnLocation;

};


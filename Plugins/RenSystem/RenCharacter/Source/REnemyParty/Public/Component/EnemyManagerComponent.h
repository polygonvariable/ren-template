// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/EnemyData.h"

// Generated Headers
#include "EnemyManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UAssetManager;
class AEnemyCharacter;
class UEnemyStorageManager;
struct FStreamableHandle;


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

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Collection")
	TMap<FGuid, TObjectPtr<AEnemyCharacter>> ActiveEnemies;

	TObjectPtr<UAssetManager> AssetManager = nullptr;
	TObjectPtr<UEnemyStorageManager> StorageManager = nullptr;


	void GetSpawningAssetIds(TArray<FPrimaryAssetId>& OutAssetIds) const;

	void SpawnEnemies();
	void SpawnEnemy(const FEnemySpawnData& Data);
	bool SpawnCondition(const FEnemySpawnData& Data) const;

	void RegisterEnemy(FGuid EnemyId, AEnemyCharacter* Character);
	void UnregisterEnemy(AEnemyCharacter* Character);

	void CleanupEnemies();
	
	// ~ Binding
	void HandleOnEnemyStateChanged(FGuid EnemyId, bool bIsAlive);
	void HandleEnemyTimestamp(const FEnemySpawnData* Data, bool bIsAlive);
	void HandleEnemyDrop(const FEnemySpawnData* Data, bool bIsAlive);
	// ~ End of Binding

private:

	TSharedPtr<FStreamableHandle> _SpawnHandle = nullptr;

};


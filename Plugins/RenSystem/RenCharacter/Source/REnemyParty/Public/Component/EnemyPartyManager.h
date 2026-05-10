// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/CharacterBase.h"
#include "Definition/AssetQuerySource.h"

// Generated Headers
#include "EnemyPartyManager.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class UPartySubsystem;
class UPartyStorageManager;
class ACharacterBase;




USTRUCT()
struct FCharacterInitializationData
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Meta = (AllowedTypes = "Asset.Character"))
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> Attributes;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FInstancedStruct> Metadata;

	bool IsValid() const
	{
		return AssetId.IsValid();
	}

};

USTRUCT()
struct FEnemyInitializationData : public FCharacterInitializationData
{

	GENERATED_BODY()

};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDataChanged);

/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UEnemyManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UEnemyManagerComponent(const FObjectInitializer& ObjectInitializer);


	UPROPERTY(BlueprintAssignable)
	FOnEnemyDataChanged OnAllEnemyDied;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDataChanged OnAnyEnemyDied;


	UPROPERTY(EditAnywhere)
	TArray<FEnemyInitializationData> SpawnData;


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

	UPROPERTY(EditAnywhere)
	EAssetQuerySource SourceType = EAssetQuerySource::Asset;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<TObjectPtr<ACharacterBase>> ActiveEnemies;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	void GetSpawningAssetIds(TArray<FPrimaryAssetId>& OutAssetIds) const;

	void SpawnEnemies();
	void SpawnEnemy(const FEnemyInitializationData& Data);

	void RegisterEnemy(ACharacterBase* Character);
	void UnregisterEnemy(ACharacterBase* Character);

	void CleanupEnemies();


	UFUNCTION()
	void HandleOnEnemyDataChanged(ACharacterBase* Character);

	void CheckAllEnemies();

private:

	FGuid _SpawnId;
	FVector _SpawnLocation;

};


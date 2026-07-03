// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Definition/CharacterInitializationData.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "PartyManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UAssetManager;
class UPartySubsystem;
class UPartyStorageManager;
class AAvatarCharacter;
struct FStreamableHandle;


/**
 *
 *
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class UPartyManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UPartyManagerComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceType==EDataSource::Static", EditConditionHides))
	TArray<FCharacterInitializationData> CharacterData;


	UFUNCTION(BlueprintCallable)
	void SpawnParty();

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
	EDataSource SourceType = EDataSource::Static;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<FPrimaryAssetId> CharacterAssetIds;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TMap<FPrimaryAssetId, TObjectPtr<AAvatarCharacter>> PartyCharacters;

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	UPROPERTY()
	TObjectPtr<UPartyStorageManager> StorageManager;

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;


	void SpawnPartyCharacters();
	void SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterInitializationData& Data);

	void RegisterCharacter(const FPrimaryAssetId& AssetId, AAvatarCharacter* Character);
	void UnregisterCharacter(const FPrimaryAssetId& AssetId);

	void RefreshPartyOrder();
	void RefreshSpawnData();
	void CleanupPartyCharacters();

	void PossessAliveCharacter();
	ACharacter* GetAliveCharacter() const;
	APlayerController* GetController() const;

private:

	TSharedPtr<FStreamableHandle> _SpawnHandle;
	FGuid _SpawnId;
	FVector _SpawnLocation;

};


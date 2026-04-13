// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Actor/CharacterBase.h"
#include "Definition/AssetQuerySource.h"

// Generated Headers
#include "PartyManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class URAssetManager;
class UPartySubsystem;
class UPartyStorage;
class ACharacterBase;


/**
 *
 *
 */
UCLASS(Meta = (BlueprintSpawnableComponent))
class UPartyManagerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	UPartyManagerComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Meta = (EditCondition = "SourceType==EAssetQuerySource::Asset", EditConditionHides))
	TArray<FCharacterData> CharacterSpawnData;


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
	EAssetQuerySource SourceType = EAssetQuerySource::Asset;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TArray<FPrimaryAssetId> CharacterAssetIds;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TMap<FPrimaryAssetId, TObjectPtr<ACharacterBase>> PartyCharacters;

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	UPROPERTY()
	TObjectPtr<UPartyStorage> PartyStorage;

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;


	void SpawnPartyCharacters();
	void SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterData& Data);

	void RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character);
	void UnregisterCharacter(const FPrimaryAssetId& AssetId);

	void RefreshPartyOrder();
	void RefreshSpawnData();
	void CleanupPartyCharacters();

	void PossessAliveCharacter();
	ACharacter* GetAliveCharacter() const;
	APlayerController* GetController() const;

private:

	FGuid _SpawnId;
	FVector _SpawnLocation;

};


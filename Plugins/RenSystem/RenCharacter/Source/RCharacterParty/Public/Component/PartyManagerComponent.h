// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Generated Headers
#include "PartyManagerComponent.generated.h"

// Forward Declarations
class UCharacterAsset;
class URAssetManager;
class UPartySubsystem;
class UPartyStorage;
class ARCharacterBase;


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

	UFUNCTION(BlueprintCallable)
	void SpawnParty();

	UFUNCTION(BlueprintCallable)
	void ClearParty();

	UFUNCTION(BlueprintCallable)
	void SwitchCharacter(int Index);


	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	UPROPERTY()
	TObjectPtr<UPartyStorage> PartyStorage;

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<ARCharacterBase>> PartyCharacters;

	TArray<FPrimaryAssetId> PartyOrder;


	void SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets);

	void SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset);
	void RemoveCharacter(ARCharacterBase* Character);

	void RegisterCharacter(ARCharacterBase* Character);
	void UnregisterCharacter(ARCharacterBase* Character);

	void HandleOnCharacterDied();

	void PossessAliveCharacter();

	ACharacter* GetAliveCharacter() const;
	APlayerController* GetControllerWithAuthority() const;

private:

	FVector _PartySpawnLocation;
	FGuid _PartySpawnId;

};


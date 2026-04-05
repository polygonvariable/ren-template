// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"
#include "InstancedStruct.h"

// Project Headers
#include "Actor/CharacterBase.h"
#include "Definition/AscensionData.h"
#include "Definition/AssetQuerySource.h"

// Generated Headers
#include "PartyManagerComponent.generated.h"

// Forward Declarations
class FObjectPreSaveContext;
class UCharacterAsset;
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


	UPROPERTY(EditAnywhere)
	EAssetQuerySource SourceType = EAssetQuerySource::Asset;

	UPROPERTY(EditAnywhere)
	TArray<FCharacterData> SpawnData;


	UFUNCTION(BlueprintCallable)
	void SpawnParty();

	void SpawnParty_Internal();


	void SpawnCharacter(const FPrimaryAssetId& AssetId, const FCharacterData& Data);

	void RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character);
	void UnregisterCharacter(const FPrimaryAssetId& AssetId);


	//UFUNCTION(BlueprintCallable)
	//void SpawnParty();

	//UFUNCTION(BlueprintCallable)
	//void ClearParty();

	//UFUNCTION(BlueprintCallable)
	//void SwitchCharacter(int Index);


	// ~ UActorComponent
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// ~ End of UActorComponent

	// ~ UObject
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
	// ~ End of UObject

protected:

	UPROPERTY()
	TObjectPtr<URAssetManager> AssetManager;

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	UPROPERTY()
	TObjectPtr<UPartyStorage> PartyStorage;

	UPROPERTY(VisibleAnywhere)
	TArray<FPrimaryAssetId> PartyOrder;


	void UpdatePartyOrder();
	void UpdateSpawnData();
	void RemovePreviousParty();


	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<ACharacterBase>> PartyCharacters;

	//TArray<FPrimaryAssetId> PartyOrder;


	//void SpawnParty_Internal(const TArray<UCharacterAsset*>& Assets);

	//void SpawnCharacter(UWorld* World, const UCharacterAsset* CharacterAsset);
	//void RemoveCharacter(ACharacterBase* Character);

	//void RegisterCharacter(ACharacterBase* Character);
	//void UnregisterCharacter(ACharacterBase* Character);

	//void HandleOnCharacterDied();

	void PossessAliveCharacter();
	ACharacter* GetAliveCharacter() const;

	APlayerController* GetControllerWithAuthority() const;

private:

	FGuid _SpawnId;
	FVector _SpawnLocation;

};


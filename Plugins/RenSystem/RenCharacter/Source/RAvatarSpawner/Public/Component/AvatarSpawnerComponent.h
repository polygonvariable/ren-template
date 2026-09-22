// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "CharacterSpawnerComponent.h"
#include "Core/Type/CharacterInitializationData.h"

// Generated Headers
#include "AvatarSpawnerComponent.generated.h"

// Forward Declarations
class UPartySubsystem;
class UPartyStorageManager;


/**
 * NOTE: Must be placed inside PlayerController
 */
UCLASS(MinimalAPI, meta = (BlueprintSpawnableComponent))
class UAvatarSpawnerComponent : public UCharacterSpawnerComponent
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (EditCondition = "SourceType==EDataSource::Static", EditConditionHides))
	TArray<FCharacterInitializationData> CharacterData;


	// ~ UActorComponent
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	UPROPERTY()
	TObjectPtr<UPartyStorageManager> StorageManager;


	APlayerController* GetController() const;
	void PossessCharacter();
	void UnPossessCharacter();

	// ~ Binding
	void HandleOnCharactersUpdated();
	// ~ End of Binding

	// ~ UCharacterSpawnerComponent
	virtual void OnSpawnStarted() override;
	virtual void OnSpawnFinished() override;

	virtual void GetCharacterData(TArray<FCharacterInitializationData>& OutData) override;
	virtual void PreRegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character) override;
	// ~ End of UCharacterSpawnerComponent
	
};

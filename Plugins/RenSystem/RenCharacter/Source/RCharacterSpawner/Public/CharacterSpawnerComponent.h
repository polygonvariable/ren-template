// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/CharacterInitializationData.h"
#include "Definition/QueryType.h"

// Generated Headers
#include "CharacterSpawnerComponent.generated.h"

// Forward Declarations
class UAssetManager;
class ACharacterBase;
struct FStreamableHandle;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UCharacterSpawnerComponent : public UActorComponent
{

	GENERATED_BODY()

public:

	RCHARACTERSPAWNER_API UCharacterSpawnerComponent();


	UPROPERTY(EditAnywhere)
	EDataSource SourceType = EDataSource::Static;


	RCHARACTERSPAWNER_API void CreateCharacters();
	RCHARACTERSPAWNER_API void RemoveCharacters();

	RCHARACTERSPAWNER_API void ShowCharacters();
	RCHARACTERSPAWNER_API void HideCharacters();

	// ~ UActorComponent
	RCHARACTERSPAWNER_API virtual void InitializeComponent() override;
	RCHARACTERSPAWNER_API virtual void UninitializeComponent() override;
	// ~ End of UActorComponent

protected:

	UPROPERTY()
	TObjectPtr<UAssetManager> AssetManager;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay)
	TMap<FPrimaryAssetId, TObjectPtr<ACharacterBase>> SpawnedCharacters;


	virtual void GetCharacterData(TArray<FCharacterInitializationData>& OutData) {};

	void SpawnCharacter(FCharacterInitializationData& Data);
	virtual void OnSpawnStarted() {};
	virtual void OnSpawnFinished() {};

	virtual void PreRegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character) {};
	void RegisterCharacter(const FPrimaryAssetId& AssetId, ACharacterBase* Character);
	void UnregisterCharacter(const FPrimaryAssetId& AssetId);

	RCHARACTERSPAWNER_API ACharacter* GetAliveCharacter() const;

	// ~ Binding
	void HandleOnCharactersLoaded(TArray<FCharacterInitializationData> CharacterData);
	// ~ End of Binding

private:

#if WITH_EDITOR
	uint64 AsyncStartTime;
#endif

	TSharedPtr<FStreamableHandle> SpawnHandle;

};


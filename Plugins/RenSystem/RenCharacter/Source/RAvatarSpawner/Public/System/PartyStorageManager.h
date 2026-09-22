// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Interface/AssetInstanceRelation.h"
#include "Core/StorageManager.h"

// Generated Headers
#include "PartyStorageManager.generated.h"

// Forward Declarations
class UPartyStorage;


/**
 *
 */
UCLASS(MinimalAPI)
class UPartyStorageManager : public UObject, public IStorageManager, public IAssetInstanceRelation
{

	GENERATED_BODY()

public:

	RAVATARSPAWNER_API FVector GetPartyLocation(const FName& Level) const;
	RAVATARSPAWNER_API void SetPartyLocation(const FName& Level, const FVector& Location);

	RAVATARSPAWNER_API TArray<FPrimaryAssetId> GetCharacters() const;
	RAVATARSPAWNER_API TArray<FPrimaryAssetId> GetTemporaryCharacters() const;
	RAVATARSPAWNER_API void GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const;

	RAVATARSPAWNER_API bool GetCharacterAtSlot(int Slot, FPrimaryAssetId& AssetId) const;
	RAVATARSPAWNER_API bool SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId);
	RAVATARSPAWNER_API bool RemoveCharacterFromSlot(int Slot);

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	// ~ End of UStorageManager

	// ~ IAssetInstanceRelation
	virtual bool HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const override;
	// ~ End of IAssetInstanceRelation

protected:

	UPROPERTY()
	TObjectPtr<UPartyStorage> LocalStorage = nullptr;

};


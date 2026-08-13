// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Delegate/GameEventDelegate.h"
#include "Core/Interface/AssetInstanceRelation.h"
#include "Interface/StorageManager.h"

// Generated Headers
#include "PartyStorageManager.generated.h"

// Module Macros
#define REN_API RCHARACTERPARTY_API

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

	FGameEventDelegate OnStorageUpdated;


	REN_API FVector GetPartyLocation(const FName& Level) const;
	REN_API void SetPartyLocation(const FName& Level, const FVector& Location);

	REN_API TArray<FPrimaryAssetId> GetCharacters() const;
	REN_API TArray<FPrimaryAssetId> GetTemporaryCharacters() const;
	REN_API void GetAllCharacters(TArray<FPrimaryAssetId>& OutCharacters) const;

	REN_API bool GetCharacterAtSlot(int Slot, FPrimaryAssetId& AssetId) const;
	REN_API bool SetCharacterAtSlot(int Slot, FPrimaryAssetId AssetId);
	REN_API bool RemoveCharacterFromSlot(int Slot);

	// ~ UStorageManager
	virtual UObject* GetStorage() const override;
	virtual void SetStorage(UObject* Storage) override;
	virtual void OnStorageLoaded(bool bIsNew) override;
	virtual FGameEventDelegate& GetOnStorageUpdated() override;
	// ~ End of UStorageManager

	// ~ IAssetInstanceRelation
	virtual bool HasLinkedInstance(const FPrimaryAssetId& AssetId, const FGuid& AssetInstanceId) const override;
	// ~ End of IAssetInstanceRelation

protected:

	UPROPERTY()
	TObjectPtr<UPartyStorage> LocalStorage = nullptr;

};


// Module Macros
#undef REN_API


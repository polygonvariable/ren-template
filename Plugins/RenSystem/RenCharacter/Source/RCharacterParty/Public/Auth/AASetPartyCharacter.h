// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AASetPartyCharacter.generated.h"

// Forwade Declarations
class UPartySubsystem;


/**
 * Auth action to add a character to the party
 */
UCLASS(NotBlueprintType)
class UAASetPartyCharacter : public UAuthAction
{

	GENERATED_BODY()

public:

	int CharacterSlot = 0;
	FPrimaryAssetId CharacterAssetId;

protected:

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	FTimerHandle TimerHandle;


	void Step_AssignCharacter();
	void Step_Finalize();

	// ~ UAuthAction
	virtual void OnStarted() override;
	virtual void OnCompleted(bool bSuccess) override;
	virtual void OnCleanup() override;
	// ~ End of UAuthAction

};


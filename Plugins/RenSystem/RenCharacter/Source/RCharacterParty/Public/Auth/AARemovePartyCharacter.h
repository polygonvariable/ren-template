// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AARemovePartyCharacter.generated.h"

// Forwade Declarations
class UPartySubsystem;


/**
 * Auth Action to remove a character from the party
 */
UCLASS(NotBlueprintType)
class UAARemovePartyCharacter : public UAuthAction
{

	GENERATED_BODY()

public:

	int CharacterSlot = 0;

protected:

	UPROPERTY()
	TObjectPtr<UPartySubsystem> PartySubsystem;

	FTimerHandle TimerHandle;


	void Step_RemoveCharacter();
	void Step_Finalize();

	// ~ UAuthAction
	virtual void OnStarted() override;
	virtual void OnCompleted(bool bSuccess) override;
	virtual void OnCleanup() override;
	// ~ End of UAuthAction

};


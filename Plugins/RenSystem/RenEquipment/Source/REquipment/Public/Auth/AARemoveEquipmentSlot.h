// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AARemoveEquipmentSlot.generated.h"

// Forward Declarations
class UEquipmentSubsystem;


/**
 * Auth action to assign equipment to slot
 */
UCLASS(NotBlueprintType)
class UAARemoveEquipmentSlot : public UAuthAction
{

	GENERATED_BODY()

public:

	FGuid OwnerInstanceId;
	FGameplayTag Slot;

protected:

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem;

	FTimerHandle TimerHandle;


	void Step_RemoveEquipment();
	void Step_Finalize();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};


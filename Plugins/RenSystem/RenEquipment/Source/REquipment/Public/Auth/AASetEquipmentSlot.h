// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "Auth/AuthAction.h"

// Generated Headers
#include "AASetEquipmentSlot.generated.h"

// Forward Declarations
class UEquipmentSubsystem;


/**
 * Auth action to assign equipment to slot
 */
UCLASS(NotBlueprintType)
class UAASetEquipmentSlot : public UAuthAction
{

	GENERATED_BODY()

public:

	FGuid OwnerInstanceId;
	FPrimaryAssetId OwnerAssetId;

	FGameplayTag Slot;

	FGuid EquipmentInstanceId;
	FPrimaryAssetId EquipmentAssetId;
	
protected:

	UPROPERTY()
	TObjectPtr<UEquipmentSubsystem> EquipmentSubsystem;

	FTimerHandle TimerHandle;


	void Step_AssignEquipment();
	void Step_Finalize();

	// ~ UAuthAction
	void OnStarted() override;
	void OnCompleted(bool bSuccess) override;
	void OnCleanup() override;
	// ~ End of UAuthAction

};


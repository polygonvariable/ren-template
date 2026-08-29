// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#include "GameplayTagContainer.h"

// Project Headers
#include "System/EquipmentController.h"

// Generated Headers
#include "EquipmentStateController.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API


/**
 *
 */
UENUM()
enum class EEquipmentState : uint8
{
	Inactive,
	Activating,
	Active,
	Deactivating
};


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentStateController : public UEquipmentController
{

	GENERATED_BODY()

public:

	DECLARE_DELEGATE_OneParam(FOnEquipmentStateChanged, UEquipmentStateController* /* Controller */);
	FOnEquipmentStateChanged OnDeactivatedDelegate;
	FOnEquipmentStateChanged OnActivatedDelegate;


	bool IsActive() const;
	bool IsActivating() const;
	bool IsDeactivating() const;
	bool IsTransitioning() const;
	bool IsInactive() const;

	EEquipmentState GetState() const;

	// ~ UEquipmentStateController
	REN_API virtual bool ActivateEquipment() override;
	REN_API virtual bool DeactivateEquipment(bool bForce = false) override;
	// ~ End of UEquipmentStateController

protected:

	void SetState(EEquipmentState InState);

	virtual void CompleteActivation();
	virtual void CompleteDeactivation();

	// ~ Bindings
	REN_API virtual void HandleOnDeactivationTagChanged(const FGameplayTag Tag, int32 NewCount);
	// ~ End of Bindings

	// ~ UEquipmentStateController
	REN_API virtual bool CanActivate() const;
	REN_API virtual void InitializeGameplayEvent() override;
	REN_API virtual void DeinitializeGameplayEvent() override;
	// ~ End of UEquipmentStateController

private:

	EEquipmentState _EquipmentState = EEquipmentState::Inactive;

};


// Module Macros
#undef REN_API


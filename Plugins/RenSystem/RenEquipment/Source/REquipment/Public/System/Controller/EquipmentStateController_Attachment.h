// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Engine Headers
#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

// Project Headers
#include "System/Controller/EquipmentStateController.h"

// Generated Headers
#include "EquipmentStateController_Attachment.generated.h"

// Module Macros
#define REN_API REQUIPMENT_API

// Forward Declarations
class UAnimInstance;
class UAnimMontage;
struct FGameplayEventData;
struct FEquipmentSocketDefinition;


/**
 *
 */
UCLASS(Abstract, MinimalAPI)
class UEquipmentAttachmentController : public UEquipmentStateController
{

	GENERATED_BODY()

public:

	// ~ UEquipmentController
	virtual bool ActivateEquipment() override;
	virtual bool DeactivateEquipment(bool bForce = false) override;
	// ~ End of UEquipmentController

#if WITH_EDITOR
	// ~ UObject
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
	// ~ End of UObject
#endif

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> EquipAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> UnequipAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> EquipmentAnimInstance = nullptr;

	bool bAttachNotifyReceived = false;
	bool bDetachNotifyReceived = false;


	virtual void LinkAnimInstance();
	virtual void UnlinkAnimInstance();

	virtual void PlayEquipAnimation();
	virtual void PlayUnequipAnimation();

	virtual void RegisterMontageEndDelegate(UAnimInstance* AnimInstance, UAnimMontage* Montage, bool bIsEquip);
	virtual void RegisterAttachmentNotify(bool bIsEquip);
	virtual void UnregisterAttachmentNotify(bool bIsEquip);

	virtual void AttachToEquipSocket();
	virtual void AttachToUnequipSocket();
	virtual void AttachToSocket(const FEquipmentSocketDefinition& Socket, const FTransform& SocketTransform);

	// ~ Binidngs
	virtual void HandleOnMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip);
	virtual void HandleOnMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void HandleOnMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted);
	// ~ End of Bindings

	// ~ UEquipmentController
	virtual void AttachEquipment() override;
	virtual void CompleteActivation() override;
	virtual void CompleteDeactivation() override;
	// ~ End of UEquipmentController

};


// Module Macros
#undef REN_API


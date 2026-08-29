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
	REN_API virtual bool ActivateEquipment() override;
	REN_API virtual bool DeactivateEquipment(bool bForce = false) override;
	// ~ End of UEquipmentController

#if WITH_EDITOR
	// ~ UObject
	REN_API virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
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


	void LinkAnimInstance();
	void UnlinkAnimInstance();

	void PlayEquipAnimation();
	void PlayUnequipAnimation();

	void RegisterMontageEndDelegate(UAnimInstance* AnimInstance, UAnimMontage* Montage, bool bIsEquip);
	void RegisterAttachmentNotify(bool bIsEquip);
	void UnregisterAttachmentNotify(bool bIsEquip);

	void AttachToEquipSocket();
	void AttachToUnequipSocket();
	void AttachToSocket(const FEquipmentSocketDefinition& Socket, const FTransform& SocketTransform);

	// ~ Binidngs
	REN_API virtual void HandleOnMontageAttachmentNotify(const FGameplayEventData* Payload, bool bIsEquip);
	REN_API virtual void HandleOnMontageEquipEnded(UAnimMontage* Montage, bool bInterrupted);
	REN_API virtual void HandleOnMontageUnequipEnded(UAnimMontage* Montage, bool bInterrupted);
	// ~ End of Bindings

	// ~ UEquipmentController
	REN_API virtual void AttachEquipment() override;
	REN_API virtual void CompleteActivation() override;
	REN_API virtual void CompleteDeactivation() override;
	// ~ End of UEquipmentController

};


// Module Macros
#undef REN_API


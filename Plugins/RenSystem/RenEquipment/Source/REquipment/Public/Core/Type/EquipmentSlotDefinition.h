// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Project Headers
#include "Core/Type/EquipmentSocketDefinition.h"

// Generated Headers
#include "EquipmentSlotDefinition.generated.h"


/**
 * 
 */
USTRUCT()
struct FEquipmentSlotDefinition
{

	GENERATED_BODY()

public:

	FEquipmentSlotDefinition() {};
	FEquipmentSlotDefinition(int InSlotId, int InInputId) : SlotId(InSlotId), InputId(InInputId) {};
	FEquipmentSlotDefinition(int InSlotId, int InInputId, FEquipmentSocketDefinition InAttachSocket, FEquipmentSocketDefinition InDetachSocket) : SlotId(InSlotId), InputId(InInputId), AttachSocket(InAttachSocket), DetachSocket(InDetachSocket) {};


	UPROPERTY(EditAnywhere)
	int SlotId = 10;

	UPROPERTY(EditAnywhere)
	int InputId = 10;

	UPROPERTY(EditAnywhere)
	FEquipmentSocketDefinition AttachSocket;

	UPROPERTY(EditAnywhere)
	FEquipmentSocketDefinition DetachSocket;


	bool IsValid() const
	{
		return SlotId > 0;
	}

};

